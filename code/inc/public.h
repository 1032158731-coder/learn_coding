#ifndef __PUBLIC_H__
#define __PUBLIC_H__


#define Version "V0.0.04.10a"


#define my_zlog_debug(zlog_flag_t, fmt, ...) 		do{zlog_debug(zlog_flag_t, "[%s] " fmt, Version, ##__VA_ARGS__);}while(0);
#define my_zlog_info(zlog_flag_t, fmt, ...) 		do{zlog_info(zlog_flag_t, "[%s] " fmt, Version, ##__VA_ARGS__);}while(0);
#define my_zlog_warn(zlog_flag_t, fmt, ...) 		do{zlog_warn(zlog_flag_t, "[%s] " fmt, Version, ##__VA_ARGS__);}while(0);
#define my_zlog_error(zlog_flag_t, fmt, ...) 		do{zlog_error(zlog_flag_t, "[%s] " fmt, Version, ##__VA_ARGS__);}while(0);
#define my_zlog_fatal(zlog_flag_t, fmt, ...) 		do{zlog_fatal(zlog_flag_t, "[%s] " fmt, Version, ##__VA_ARGS__);}while(0);


typedef unsigned char UInt8;
typedef unsigned short int UInt16;
typedef unsigned int UInt32; 
typedef char Int8;
typedef short int Int16;
typedef int Int32;


// 日志
#define RUN_LOG_FILE   "/usr/local/emsApp/bin/log/run_log.txt"
#define MSG_LOG_FILE   "/usr/local/emsApp/bin/msg_log.txt"
#define RUN_LOG_CAPCITY  (5242880) // 5 * 1024 * 1024 = 5M
#define MSG_LOG_CAPCITY  (2097152) // 2 * 1024 * 1024 = 2M


#define MSG_MAX_LEN (2048)


#define DW_ERR    (-1)
#define DW_OK     (0)
#define DW_PRO    (1)

#define LOG_DEBUG	 	0
#define LOG_INFO 		1
#define LOG_WARNING	 	2
#define LOG_ERROR 		3
#define LOG_CRITICAL	4


typedef enum
{
    TCP_RECV = 0,
    TCP_SEND = 1
}MsgTypeEnum;
	
extern void WriteLog(const char *stringFormat, ...);
extern void WriteMsg(const MsgTypeEnum enType, const unsigned char *pMsg, const int len);
	
#define SYS_LOG(level, format,...) do{WriteLog("[%s|%s,%s,%d]"format"\n", level, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__);}while(0);


// 可以从配置文件中读取日志级别
//read_log_level_from_config();


static int g_current_log_level = LOG_DEBUG;  // 默认日志级别为 INFO

#define COLOR_NONE      "\033[0m"
#define COLOR_DEBUG     "\033[36m"   // 青色
#define COLOR_INFO      "\033[32m"   // 绿色
#define COLOR_WARNING   "\033[33m"   // 黄色
#define COLOR_ERROR     "\033[31m"   // 红色
#define COLOR_CRITICAL  "\033[41;37m" // 红底白字

#define log_debug(format, ...) do { if (g_current_log_level <= LOG_DEBUG) printf(COLOR_DEBUG "[DEBUG] File: %s, Line: %d " format COLOR_NONE, __FILE__, __LINE__, ##__VA_ARGS__); } while(0)
#define log_info(format, ...) do { if (g_current_log_level <= LOG_INFO) printf(COLOR_INFO "[INFO] File: %s, Line: %d " format COLOR_NONE, __FILE__, __LINE__, ##__VA_ARGS__); } while(0)
#define log_warning(format, ...) do { if (g_current_log_level <= LOG_WARNING) printf(COLOR_WARNING "[WARNING] File: %s, Line: %d " format COLOR_NONE, __FILE__, __LINE__, ##__VA_ARGS__); } while(0)
#define log_error(format, ...) do { if (g_current_log_level <= LOG_ERROR) printf(COLOR_ERROR "[ERROR] File: %s, Line: %d " format COLOR_NONE, __FILE__, __LINE__, ##__VA_ARGS__); } while(0)
#define log_critical(format, ...) do { if (g_current_log_level <= LOG_CRITICAL) printf(COLOR_CRITICAL "[CRITICAL] File: %s, Line: %d " format COLOR_NONE, __FILE__, __LINE__, ##__VA_ARGS__); } while(0)


#endif