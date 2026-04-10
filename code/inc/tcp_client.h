#ifndef __TCP_CLIENT_H__
#define __TCP_CLIENT_H__

#include "user.h"
#include "public.h"
#include "multithreadmutex.h"



class TcpClient
{
public:
	static TcpClient *GetInstance();
    static void Destroy(); 

private:

	//初始化
	Int32 Init();
private:
	
	static TcpClient *m_pInstance ;
	static MultiThreadMutex m_mutexOfInstance;

};

#endif 


