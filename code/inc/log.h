#ifndef __LOG_H__
#define __LOG_H__

#include <stdio.h>
#include <stdarg.h>
#include "public.h"
#include "multithreadmutex.h"
#include "user.h"
#include "zlog.h"  // 添加zlog头文件

// 声明一个全局变量，供整个项目使用
extern zlog_category_t *g_zlog_flag;  // 恢复zlog相关声明

class Log
{
public:
    static Log *GetInstance();
    static void Destroy(); 

    Int8 Init();

private:

    static Log *m_instance;
    static MultiThreadMutex m_mutex;

    Log();
    ~Log();

public:

    void WriteLog(const char *pLog);
    void WriteMsg(const char *pMsg);

private:

    Int8 IsFileFull(const char* file_name, int max_capacity);

    MultiThreadMutex m_mutexOfLog;
    MultiThreadMutex m_mutexOfMsg;

};



#endif