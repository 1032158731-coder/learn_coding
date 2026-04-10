#ifndef __USER_H__
#define __USER_H__

#include <errno.h> 
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <cstdlib>
#include <dirent.h>  // 包含必要的头文件

#include "public.h"
#include "mytypes.h"
#include "multithreadmutex.h"




class User
{
public:
    static User *GetInstance();
    static void Destroy(); 
	
	void DisplayList(P_Node head );
	P_Node NewNode(dataType *newdata);
	void InsList(P_Node Prev ,P_Node new_p,P_Node Next);
	int SearchForFiles(P_Node head ,const char*SearchTypes, const char*SearchPath);	

private:
	
    static User *m_pInstance;
	static MultiThreadMutex m_mutexOfInstance;	


private:
	User();
	~User();

	int Init(); 			 //初始化	

	
};


#endif