#ifndef __SCOPEDLOCK_H__
#define __SCOPEDLOCK_H__

template<typename T>
class ScopedLock
{
public:
	ScopedLock(const T &lock)
		:m_lock(lock)
	{
	    lock.Lock();
	}

    ~ScopedLock()
    {
        m_lock.Unlock();
    }
	
private:
	ScopedLock(const ScopedLock &rhs);
	ScopedLock &operator=(const ScopedLock &rhs);

	const T &m_lock;
};

template<typename T>
class ScopedUnlock
{
public:
	ScopedUnlock(const T &lock)
		:m_lock(lock)
	{
	    lock.Unlock();
	}

    ~ScopedUnlock()
	{
	    m_lock.Lock();
	}
	
private:
	ScopedUnlock(const ScopedUnlock &rhs);
	ScopedUnlock &operator=(const ScopedUnlock &rhs);

	const T &m_lock;
};

template<typename T>
class TryScopedLock
{
public:
	TryScopedLock(const T &lock, bool locked = true)
		:m_lock(lock)
		,m_locked(locked && lock.TryLock())
	{
	    
	}

	~TryScopedLock()
    {
        if(m_locked)
        {
            m_lock.Unlock();
        }
    }

	bool IsLocked()const
    {
        return m_locked;
    }

    bool RetryLock()const
    {
        m_locked = m_lock.TryLock();
        return m_locked;
    }
	
private:
	TryScopedLock(const TryScopedLock &rhs);
	TryScopedLock &operator=(const TryScopedLock &rhs);

	const T &m_lock;
	mutable bool m_locked;
};

#endif //!__SCOPEDLOCK_H__

