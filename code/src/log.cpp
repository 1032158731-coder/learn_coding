#include "log.h"
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include "public.h"
#include "scopedlock.h"

Log *Log::m_instance = NULL;
MultiThreadMutex Log::m_mutex;
zlog_category_t *g_zlog_flag = NULL;  // 保持声明以兼容现有代码

void WriteLog(const char *format, ...)
{
    char buff[MSG_MAX_LEN] = {0};
    va_list va;
    Log *pLog = Log::GetInstance();
    if(NULL == pLog)
    {
        printf("Get the instance of log failed!\n");
        return;
    }

    memset(buff, 0, sizeof(buff));
    va_start(va, format);
    int n = vsnprintf(buff, MSG_MAX_LEN, format, va);
    if((n > 0) && (n < MSG_MAX_LEN))
    {
        pLog->WriteLog(buff);
    }
    va_end(va);
}

static const char *msg_type[2] = {"recv", "send"};
void WriteMsg(const MsgTypeEnum enType, const unsigned char *pMsg, const int len)
{
    Log *pLog = Log::GetInstance();
    if((NULL == pLog) || (NULL == pMsg) || (0 >= len))
    {
        printf("Write tcp msg error\n");
        return;
    }
    
    char buff[MSG_MAX_LEN] = {0};
    memset(buff, 0, sizeof(buff));
    struct tm currentTime;
    time_t t = time(NULL);
	localtime_r(&t, &currentTime);  //线程安全函数
    sprintf(buff, "%04d-%02d-%02d %02d:%02d:%02d %s %03d bytes:", currentTime.tm_year + 1900, currentTime.tm_mon + 1, currentTime.tm_mday, currentTime.tm_hour, currentTime.tm_min, currentTime.tm_sec, msg_type[enType], len);

    int idx_b = strlen(buff);
    int idx_m = 0;
    for(idx_m = 0; (idx_m < len) && (idx_b < 1020); idx_m++, idx_b += 3)
    {
        sprintf(buff + idx_b, "%02X ", pMsg[idx_m]);
    }

    buff[idx_b] = 0;
    pLog->WriteMsg(buff);
}

Log::Log()
    :m_mutexOfLog()
    ,m_mutexOfMsg()
{
    
}

Log::~Log()
{
   // zlog_fini();//移除zlog相关代码，因为没有链接库
}

Int8 Log::Init()
{
	printf("Log init\n");  // 简化初始化输出
	
    // 移除zlog相关初始化代码，因为已在main中处理
    printf("Log init successfully\n");
    return DW_OK;
}

Log *Log::GetInstance()
{
    if(NULL == m_instance)
    {
        const ScopedLock<MultiThreadMutex> lock(m_mutex);
        if(NULL == m_instance)
        {
            m_instance = new Log();
            if(DW_OK != m_instance->Init())
            {
                delete m_instance;
                m_instance = NULL;
                return NULL;
            }
        }
    }
    
    return m_instance;
}

void Log::Destroy()
{
    const ScopedLock<MultiThreadMutex> lock(m_mutex);
    if(NULL != m_instance)
    {
        delete m_instance;
        m_instance = NULL;
    }
}




Int8 Log::IsFileFull(const char* file_name, int max_capacity)
{
    struct stat info;
	if (stat(file_name, &info) == 0)
	{
		if (info.st_size > max_capacity)
		{
            char back_file[32] = {0};
            sprintf(back_file, "%s.back", file_name);
			rename(file_name, back_file);
            printf("Log file:%s is full\n", file_name);  // 使用标准输出替代log_error
            return DW_PRO;
		}
	}
	
    return DW_OK;
}

void Log::WriteLog(const char *pLog)
{
    struct tm currentTime;
	time_t t;

    const ScopedLock<MultiThreadMutex> lock(m_mutexOfLog);
    IsFileFull(RUN_LOG_FILE, RUN_LOG_CAPCITY);
	FILE *file = fopen(RUN_LOG_FILE, "a+");
    if(NULL == file )
    {
        return;
    }

    t = time(NULL);
	localtime_r(&t, &currentTime);  //线程安全函数
	fprintf(file, "%04d-%02d-%02d %02d:%02d:%02d", currentTime.tm_year + 1900, currentTime.tm_mon + 1, currentTime.tm_mday, currentTime.tm_hour, currentTime.tm_min, currentTime.tm_sec);
	fprintf(file, "%s", pLog);
	fclose(file);
}

void Log::WriteMsg(const char *pMsg)
{
    const ScopedLock<MultiThreadMutex> lock(m_mutexOfMsg);
	IsFileFull(MSG_LOG_FILE, MSG_LOG_CAPCITY);
	FILE *file = fopen(MSG_LOG_FILE, "a+");
	if ((file == NULL) || (NULL == pMsg))
	{
		return;
	}
    
    fprintf(file, "%s\n", pMsg);
	fclose(file);
}



