#include "scopedLock.h"
#include "application.h"
#include "write_read_ini.h"
#include "ifstearm.h"
#include "log.h"


Application *Application::m_pInstance = NULL;
MultiThreadMutex Application::m_mutexOfInstance;


Application *Application::GetInstance()
{
    if(NULL == m_pInstance)
    {
        const ScopedLock<MultiThreadMutex> lock(m_mutexOfInstance);
        if(NULL == m_pInstance)
        {
            m_pInstance = new Application();
            if(DW_OK != m_pInstance->Init())
            {
                delete m_pInstance;
                m_pInstance = NULL;
            }
        }
    }
    
    return m_pInstance;
}

void Application::Destroy()
{
    const ScopedLock<MultiThreadMutex> lock(m_mutexOfInstance);
    if(NULL != m_pInstance)
    {
        delete m_pInstance;
        m_pInstance = NULL;
    }

}

Application::Application()
{

}

Application::~Application()
{

}


void *Application::Test(void *pArg)
{
	while (1)
	{
		log_debug("当前线程运行中！\n");
		log_debug("写代码写代码 ing... \n");	
		sleep(1);
	}
}


int Application::Init()
{

	return DW_OK;

}

void Application::Task_50ms()
{


}

void Application::Task_100ms()
{

	//LcdInit::GetInstance()->DisplayImages(bmphead->next->data.PathName, 0, 0);  
	//bmphead = bmphead->next;
	log_debug("Test ==== 5555\n");

}

void Application::Task_250ms()
{


}

void Application::Task_500ms()
{
    WriteHelloWorldToFile();  // 调用新的私有成员函数
}

void Application::Task_1000ms()
{


}


void Application::Task_60s()
{


}


void Application::Task_300s()
{


}


void Application::WriteHelloWorldToFile()
{
    // 写入HelloWorld到文件
    std::ofstream file("helloworld.txt", std::ios::app); // 使用追加模式，防止覆盖之前的内容
    if(file.is_open()) { 
        file << "HelloWorld" << std::endl;
        file.close();
        log_info("Successfully wrote 'HelloWorld' to helloworld.txt");
    } else { 
        log_error("Failed to open helloworld.txt for writing");
    }
}

void Application::Run()
{
    /*
    if (WtReadIni::GetInstance()->write_ini("config.ini", "Settings", "theme", "=====")) 
    {
        log_error("Write INI error!");
        return ;
    }
    else
    {
        fprintf(stderr, "写入文件成功！\n");
    }
    */
    //LcdInit::GetInstance()->DisplayImages("./picture/xiao.bmp", 0, 0);
    //sleep(1);
    //LcdInit::GetInstance()->DisplayImages("./picture/zhenxiang.bmp", 0, 0);  

    TcpServer Abn;
    Abn.test();

    pthread_t thread ;
    int ret = pthread_create(&thread, NULL , Application::Test , this );
    if(DW_OK != ret)
    {
        log_error("create read thread failed! Error No: %d \n",ret);
        return ;		
    }
    // 将线程设置为 detached 状态
    ret = pthread_detach(thread);
    if (ret != 0) {
        log_error("pthread_detach");
        exit(1);
    }

    
    //bmphead =  User::GetInstance()->NewNode(NULL);
    //User::GetInstance()->SearchForFiles(bmphead ,".bmp", "./picture");

    IniFileHandler A;
    A.main_test();

    //return;
    while (1)
    {
        usleep(50 * 1000);
        Task_50ms();

        m_u16Timer100ms++;
        if(m_u16Timer100ms >=2)
        {
            Task_100ms();
            m_u16Timer100ms = 0;
        }
        
        m_u16Timer250ms++;
        if(m_u16Timer250ms >= 5)
        {
            Task_250ms();
            m_u16Timer250ms = 0;
        }
        
        m_u16Timer500ms++;
        if(m_u16Timer500ms >= 10)
        {
            Task_500ms();
            m_u16Timer500ms = 0;
        }
        
        m_u16Timer1000ms++;	
        if(m_u16Timer1000ms >= 20)
        {
            Task_1000ms();
            m_u16Timer1000ms = 0;
        }
        
        m_u16Timer60s++;	
        if(m_u16Timer60s >= 1200)
        {
            Task_60s();
            m_u16Timer60s = 0;
        }
        
        m_u16Timer300s++;			
        if(m_u16Timer300s >= 6000)
        {
            Task_300s();
            m_u16Timer300s = 0;
        }
    }
}