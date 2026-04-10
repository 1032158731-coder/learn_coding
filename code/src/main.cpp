#include <iostream>
#include <fstream>
#include <string>

#include "json.hpp""

using json = nlohmann::json;

int main() {
    std::ifstream file("/usr/local/emsApp/bin/linkconfig.json", std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "❌ 无法打开文件 config.json" << std::endl;
        return 1;
    }

    try {
        json data = json::parse(file);

        // 1. 遍历顶层所有键（设备路径）
        for (auto& [port_path, port_config] : data.items()) {
            std::cout << "📌 端口: " << port_path << std::endl;

            // 2. 检查是否为 null
            if (port_config.is_null()) {
                std::cout << "   ⚠️ 无配置 (null)\n" << std::endl;
                continue;  // 跳过，处理下一个端口
            }

            // ----- 3. 读取 parameter 参数 -----
            if (port_config.contains("parameter")) {
                auto& param = port_config["parameter"];
                std::cout << "   🔧 参数配置:" << std::endl;
                std::cout << "      波特率: " << param.value("baud", "未知") << std::endl;
                std::cout << "      校验位: " << param.value("parity", "未知") << std::endl;
                std::cout << "      数据位: " << param.value("data_bit", "未知") << std::endl;
                std::cout << "      停止位: " << param.value("stop_bit", "未知") << std::endl;
                std::cout << "      从站地址: " << param.value("slave", "未知") << std::endl;
                std::cout << "      重试次数: " << param.value("retries", "未知") << std::endl;
                std::cout << "      超时(ms): " << param.value("timeout", "未知") << std::endl;
                std::cout << "      周期(ms): " << param.value("cycle", "未知") << std::endl;
            }

            // ----- 4. 读取 device 对象（里面每个子设备键名也是动态的）-----
            if (port_config.contains("device")) {
                auto& devices = port_config["device"];
                std::cout << "   📟 挂载设备数量: " << devices.size() << std::endl;

                // 遍历 devices 下的每一个设备
                for (auto& [dev_key, dev_info] : devices.items()) {
                    std::cout << "      ┌─ 设备键: " << dev_key << std::endl;
                    std::cout << "      ├─ 名称: " << dev_info.value("name", "?") << std::endl;
                    std::cout << "      ├─ 配置文件: " << dev_info.value("file", "?") << std::endl;
                    std::cout << "      ├─ 从站地址: " << dev_info.value("slave", "?") << std::endl;
                    std::cout << "      ├─ H_Registers: " << dev_info.value("H_Registers", "?") << std::endl;
                    std::cout << "      ├─ M_Registers: " << dev_info.value("M_Registers", "?") << std::endl;
                    std::cout << "      └─ L_Registers: " << dev_info.value("L_Registers", "?") << std::endl;
                }
            }
            std::cout << std::endl;  // 空行分隔不同端口
        }

    } catch (const json::parse_error& e) {
        std::cerr << "❌ JSON 解析失败: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "❌ 运行时错误: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

/*
#include <iostream>
#include <thread>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dlfcn.h>
#include <pthread.h>  // 添加pthread头文件
#include "application.h"
#include "log.h"
#include "write_read_ini.h"

// 声明全局变量
extern zlog_category_t *g_zlog_flag;

int main()
{



    
    printf("This is a tcp client !\n");
    
    // 初始化日志系统
    Log *log = Log::GetInstance();
    if (log == NULL) {
        printf("Failed to initialize log system\n");
        return -1;
    }
    
    // 初始化全局日志标志，模拟zlog_category_t的初始化
    // 原来的代码是: g_zlog_flag = zlog_get_category("my_cat");
    // 由于我们使用了替代实现，简单赋值即可
    g_zlog_flag = (zlog_category_t*)1; // 简单赋值以通过编译
    
    // 写入一些日志信息，使用zlog.h中定义的日志功能
    zlog_info(g_zlog_flag, "Application started successfully");
    zlog_debug(g_zlog_flag, "Debug: Main function initialized");
    zlog_warn(g_zlog_flag, "Warning: This is a sample warning message");
    zlog_error(g_zlog_flag, "Error: Sample error message for testing");
    
    // 调用写入默认配置的函数
    WtReadIni::GetInstance()->writeDefaultConfig();
    
    Application *pApp = Application::GetInstance();
    if(pApp == NULL)
    {
        zlog_error(g_zlog_flag, "Get the instance of Application failed!");
        return -1;
    }
    
    pthread_t tid;
    pthread_create(&tid, NULL, pApp->Test, NULL);
    
    while(1)
    {
        sleep(5);
        sleep(5);
        pApp->Task_500ms();
        sleep(5);
        pApp->Task_500ms();
        zlog_info(g_zlog_flag, "Application is running...");  // 定期写入运行日志
    }    
     

    
    return 0;
}
    */