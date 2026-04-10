#ifndef __APPLICATION__
#define __APPLICATION__

#include "user.h"
#include "public.h"
#include "multithreadmutex.h"
#include "lcd_init.h"

#include "tcp_server.h"

#include <fstream>  // 添加文件操作头文件




class Application
{
public:
    static Application *GetInstance();
    static void Destroy(); 

    void Run();

    static void *Test(void *pArg);  //线程函数，改为公共方法
    void Task_500ms();              // 改为公共方法

private:

	Application();
	~Application();

    void WriteHelloWorldToFile();      // 新增私有成员函数，用于写入HelloWorld到文件

	int Init(); 			 //初始化

	void Task_50ms();
	void Task_100ms();	
	void Task_250ms();		
	void Task_1000ms();
	void Task_60s();
	void Task_300s();			

private:

	P_Node bmphead;


    UInt16 m_u16Timer100ms;                 //100ms定时任务计时器
    UInt16 m_u16Timer250ms;                 //250ms定时任务计时器
    UInt16 m_u16Timer500ms;                 //500ms定时任务计时器
    UInt16 m_u16Timer1000ms;                //1s定时任务计时器
    UInt16 m_u16Timer60s;                   //60s定时任务计时器
    UInt16 m_u16Timer300s;                  //300s定时任务计时器


    static Application *m_pInstance;
	static MultiThreadMutex m_mutexOfInstance;

};



#endif