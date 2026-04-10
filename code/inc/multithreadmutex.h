#ifndef __MULTITHREADMUTEX_H__
#define __MULTITHREADMUTEX_H__

#include <pthread.h>

class MultiThreadMutex
{
public:
    MultiThreadMutex();
	~MultiThreadMutex();

	void Lock()const;
    bool TryLock()const;
    void Unlock()const;
	
private:
	//禁止拷贝
	MultiThreadMutex(const MultiThreadMutex &rhs);
	MultiThreadMutex &operator=(const MultiThreadMutex &rhs);
	
	mutable pthread_mutex_t m_mutex;
};

#endif  //!__MULTITHREADMUTEX_H__
