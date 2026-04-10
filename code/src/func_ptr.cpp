#include "func_ptr.h"
#include "scopedLock.h"

// 定义错误处理函数的类型  
typedef void (*ErrorHandler)(int errorCode);  

FuncPtr *FuncPtr::m_pInstance = NULL;
MultiThreadMutex FuncPtr::m_mutexOfInstance;


FuncPtr *FuncPtr::GetInstance()
{
    if(NULL == m_pInstance)
    {
        const ScopedLock<MultiThreadMutex> lock(m_mutexOfInstance);
        if(NULL == m_pInstance)
        {
            m_pInstance = new FuncPtr();
            if(DW_OK != m_pInstance->FuncPtrRun())
            {
                delete m_pInstance;
                m_pInstance = NULL;
            }
        }
    }
    
    return m_pInstance;
}

void FuncPtr::Destroy()
{
    const ScopedLock<MultiThreadMutex> lock(m_mutexOfInstance);
    if(NULL != m_pInstance)
    {
        delete m_pInstance;
        m_pInstance = NULL;
    }
}


FuncPtr::FuncPtr()
{


}

FuncPtr::~FuncPtr()
{



}




/**********************************************************************************************************
** Function name	 : H2N_U32  
** Descriptions	     : 本地字节序转网络字节序，低字节在前高字节在后，将32位整型转换成4字节数组
** parameters	     : pMsg：接受数组   value：待转换的数
** Returned value 	 :  
***********************************************************************************************************/
// 错误处理函数  
void FuncPtr::handleError1(int errorCode) {  
    printf("Error %d: Something went wrong in part 1.\n", errorCode);  
}  
  
void FuncPtr::handleError2(int errorCode) {  
    printf("Error %d: Problem in part 2.\n", errorCode);  
}  
  
void FuncPtr::handleError3(int errorCode) {  
    printf("Error %d: Oh no, part 3!\n", errorCode);  
}  



int FuncPtr::FuncPtrRun(void)
{
    // 创建函数指针数组  
    ErrorHandler errorHandlers[] = {FuncPtr::handleError1, FuncPtr::handleError2, FuncPtr::handleError3};  
    int numHandlers = sizeof(errorHandlers) / sizeof(errorHandlers[0]);  
    
    // 假设我们得到了一个错误码  
    int errorCode = 0;  
    
    // 检查错误码是否在处理范围内  
    if (errorCode >= 0 && errorCode < numHandlers) {  
        // 调用对应的错误处理函数  
        errorHandlers[errorCode](errorCode);  
    } else {  
        printf("Unknown error code: %d\n", errorCode);  
    } 

    return DW_OK;
}




