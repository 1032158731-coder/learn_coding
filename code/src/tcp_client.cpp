#include "scopedLock.h"
#include "tcp_client.h"
#include "public.h"

TcpClient *TcpClient::m_pInstance = NULL;
MultiThreadMutex TcpClient::m_mutexOfInstance;

TcpClient *TcpClient::GetInstance()
{
    if(NULL == m_pInstance)
    {
        const ScopedLock<MultiThreadMutex> lock(m_mutexOfInstance);
        if(NULL == m_pInstance)
        {
            m_pInstance = new TcpClient();
            if(DW_OK != m_pInstance->Init())
            {
                delete m_pInstance;
                m_pInstance = NULL;
            }
        }
    }
    
    return m_pInstance;
}

void TcpClient::Destroy()
{

    const ScopedLock<MultiThreadMutex> lock(m_mutexOfInstance);
    if(NULL != m_pInstance)
    {
        delete m_pInstance;
        m_pInstance = NULL;
    }


}


Int32 TcpClient::Init()
{
	return DW_OK;
}


