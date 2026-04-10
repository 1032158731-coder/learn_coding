#include "multithreadmutex.h"

MultiThreadMutex::MultiThreadMutex()
{
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutexattr_setprotocol(&attr, PTHREAD_PRIO_INHERIT);
    pthread_mutex_init(&m_mutex, &attr);
    pthread_mutexattr_destroy (&attr);
}

MultiThreadMutex::~MultiThreadMutex()
{
    pthread_mutex_destroy(&m_mutex);
}

void MultiThreadMutex::Lock()const
{
    pthread_mutex_lock(&m_mutex);
}

bool MultiThreadMutex::TryLock()const
{
    return (0 == pthread_mutex_trylock(&m_mutex));
}

void MultiThreadMutex::Unlock()const
{
    pthread_mutex_unlock(&m_mutex);
}

