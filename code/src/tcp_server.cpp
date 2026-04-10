#include <iostream>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <thread>
#include <mutex>
#include <atomic>
#include <queue>
#include <condition_variable>
#include <fcntl.h>
#include <algorithm> 
#include <signal.h>  // 添加信号处理头文件


#include "tcp_server.h"

// 定义SO_REUSEPORT（如果系统不支持）
#ifndef SO_REUSEPORT
#define SO_REUSEPORT SO_REUSEADDR
#endif

#define PORT 9527
#define BUFFER_SIZE 4096
#define MAX_CLIENTS 100
#define THREAD_POOL_SIZE 10

// 线程安全的输出
std::mutex cout_mutex;
#define SAFE_COUT(message) { \
    std::lock_guard<std::mutex> lock(cout_mutex); \
    std::cout << message << std::endl; \
}

// 客户端信息结构
struct ClientInfo {
    int socket;
    sockaddr_in address;
    std::string ip;
    uint16_t port;
    std::thread::id thread_id;
    time_t connect_time;
};

// 线程池任务队列
std::queue<int> client_queue;
std::mutex queue_mutex;
std::condition_variable condition;
std::atomic<bool> stop_server{false};

// 客户端列表管理
std::mutex clients_mutex;
std::vector<ClientInfo> clients;

// 设置非阻塞socket
bool TcpServer::set_nonblocking(int sock) {
    int flags = fcntl(sock, F_GETFL, 0);
    if (flags == -1) return false;
    return fcntl(sock, F_SETFL, flags | O_NONBLOCK) != -1;
}
void handle_client(int client_socket);

// 处理客户端连接的线程函数
void handle_client(int client_socket) {
    ClientInfo client_info;
    char buffer[BUFFER_SIZE] = {0};
    
    // 获取客户端信息
    sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    getpeername(client_socket, (sockaddr*)&client_addr, &addr_len);
    
    client_info.socket = client_socket;
    client_info.address = client_addr;
    client_info.ip = inet_ntoa(client_addr.sin_addr);
    client_info.port = ntohs(client_addr.sin_port);
    client_info.thread_id = std::this_thread::get_id();
    client_info.connect_time = time(nullptr);
    
    // 添加到客户端列表
    {
        std::lock_guard<std::mutex> lock(clients_mutex);
        clients.push_back(client_info);
    }
    
    SAFE_COUT("[" << client_info.thread_id << "] Client connected: " 
               << client_info.ip << ":" << client_info.port);
    
    // 发送欢迎消息
    std::string welcome_msg = "Welcome to TCP server! Type 'exit' to disconnect.\r\n";
    send(client_socket, welcome_msg.c_str(), welcome_msg.size(), 0);
    
    // 与客户端通信循环
    while (!stop_server) {
        // 清空缓冲区
        memset(buffer, 0, BUFFER_SIZE);
        
        // 接收数据
        ssize_t bytes_read = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
        
        if (bytes_read > 0) {
            // 成功接收到数据
            buffer[bytes_read] = '\0';
            
            // 移除回车换行符
            if (buffer[bytes_read - 1] == '\n') buffer[bytes_read - 1] = '\0';
            if (buffer[bytes_read - 2] == '\r') buffer[bytes_read - 2] = '\0';
            
            SAFE_COUT("[" << client_info.thread_id << "] Received from " 
                       << client_info.ip << ":" << client_info.port 
                       << " (" << bytes_read << " bytes): " << buffer);
            
            // 检查退出命令
            if (strcmp(buffer, "exit") == 0) {
                SAFE_COUT("[" << client_info.thread_id << "] Client requested disconnect");
                break;
            }
            
            // 构造响应
            std::string response = "Server response: ";
            response += buffer;
            response += "\r\n";
            
            // 发送响应
            send(client_socket, response.c_str(), response.size(), 0);
        } 
        else if (bytes_read == 0) {
            // 客户端关闭连接
            SAFE_COUT("[" << client_info.thread_id << "] Client disconnected");
            break;
        } 
        else {
            // 错误处理
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                // 没有数据可读，短暂休眠后继续
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                continue;
            }
            
            // 其他错误
            SAFE_COUT("[" << client_info.thread_id << "] Recv error: " << strerror(errno));
            break;
        }
    }
    
    // 关闭客户端连接
    close(client_socket);
    
    // 从客户端列表中移除
    {
        std::lock_guard<std::mutex> lock(clients_mutex);
        clients.erase(std::remove_if(clients.begin(), clients.end(), 
            [client_socket](const ClientInfo& ci) { 
                return ci.socket == client_socket; 
            }), 
            clients.end());
    }
}

void worker_thread() ;

// 线程池工作线程
void worker_thread() {
    while (!stop_server) {
        int client_socket = -1;
        
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            condition.wait(lock, []{ 
                return !client_queue.empty() || stop_server; 
            });
            
            if (stop_server) break;
            
            client_socket = client_queue.front();
            client_queue.pop();
        }
        
        if (client_socket != -1) {
            handle_client(client_socket);
        }
    }
}

void signal_handler(int signal);

// 信号处理函数
void signal_handler(int signal) {
    SAFE_COUT("\nReceived signal " << signal << ", shutting down server...");
    stop_server = true;
    condition.notify_all(); // 唤醒所有线程
}

// 打印服务器状态
void TcpServer::print_server_status() {
    std::lock_guard<std::mutex> lock(clients_mutex);
    SAFE_COUT("\n===== Server Status =====");
    SAFE_COUT("Active clients: " << clients.size());
    SAFE_COUT("Thread pool: " << THREAD_POOL_SIZE << " threads");
    
    for (const auto& client : clients) {
        time_t now = time(nullptr);
        double duration = difftime(now, client.connect_time);
        
        SAFE_COUT(" - " << client.ip << ":" << client.port 
                   << " (Thread: " << client.thread_id 
                   << ", Connected: " << static_cast<int>(duration) << "s)");
    }
    
    SAFE_COUT("=========================");
}

int TcpServer::test() {
    int server_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    
    // 注册信号处理
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    
    // 1. 创建socket文件描述符
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    // 2. 设置套接字选项（避免地址占用错误）
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    
    // 设置非阻塞模式
    if (!set_nonblocking(server_fd)) {
        perror("fcntl failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    
    // 3. 绑定地址和端口
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    
    // 4. 开始监听
    if (listen(server_fd, SOMAXCONN) < 0) {
        perror("listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    
    SAFE_COUT("Server listening on port " << PORT);
    SAFE_COUT("Thread pool size: " << THREAD_POOL_SIZE);
    SAFE_COUT("Press Ctrl+C to stop the server\n");
    
    // 创建线程池
    std::vector<std::thread> thread_pool;
    for (int i = 0; i < THREAD_POOL_SIZE; ++i) {
        thread_pool.emplace_back(worker_thread);
    }
    
    // 主循环：接受客户端连接
    while (!stop_server) {
        // 接受新连接
        int new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        
        if (new_socket < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                // 没有新连接，短暂休眠后继续
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                
                // 定期打印服务器状态
                static int counter = 0;
                if (++counter % 50 == 0) {
                    print_server_status();
                }
                
                continue;
            }
            
            perror("accept failed");
            continue;
        }
        
        // 设置新socket为非阻塞
        if (!set_nonblocking(new_socket)) {
            perror("fcntl for client socket failed");
            close(new_socket);
            continue;
        }
        
        // 将新客户端添加到任务队列
        {
            std::lock_guard<std::mutex> lock(queue_mutex);
            client_queue.push(new_socket);
        }
        condition.notify_one();
    }
    
    // 清理工作
    SAFE_COUT("Shutting down server...");
    
    // 关闭所有客户端连接
    {
        std::lock_guard<std::mutex> lock(clients_mutex);
        for (auto& client : clients) {
            close(client.socket);
        }
        clients.clear();
    }
    
    // 关闭服务器socket
    close(server_fd);
    
    // 唤醒并等待所有工作线程结束
    condition.notify_all();
    for (auto& t : thread_pool) {
        if (t.joinable()) {
            t.join();
        }
    }
    
    SAFE_COUT("Server stopped successfully");
    return 0;
}