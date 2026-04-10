#ifndef __WRITE_READ_INI_H__
#define __WRITE_READ_INI_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "multithreadmutex.h"
#include "public.h"


class WtReadIni
{
public:
	static WtReadIni *GetInstance();
    static void Destroy(); 

	int write_ini(const char *filename, const char *section, const char *key, const char *value);
	
	// 添加写入默认配置的函数
	int writeDefaultConfig();

private:
	
	static WtReadIni *m_pInstance;
	static MultiThreadMutex m_mutexOfInstance;

    int Init();  			//初始化

};


#endif