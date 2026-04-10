#ifndef __FUNC_PTR__
#define __FUNC_PTR__

#include "user.h"
#include "public.h"
#include "multithreadmutex.h"



class  FuncPtr
{
public:

    static FuncPtr *GetInstance();
    static void Destroy();    

public:

    int FuncPtrRun(void);

private:

    static void handleError1(int errorCode);
    static void handleError2(int errorCode);
    static void handleError3(int errorCode);



    static FuncPtr *m_pInstance;
	static MultiThreadMutex m_mutexOfInstance;

    FuncPtr();
    ~FuncPtr();



};




#endif



