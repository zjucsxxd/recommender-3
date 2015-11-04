// Copyright (c) 2013, Alibaba Inc.
// All right reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2013/07/29
// Description:

#ifndef STONE_THREADING_SYNC_LOCK_H
#define STONE_THREADING_SYNC_LOCK_H

#include <pthread.h>
#include "stone/base/error_check.h"
#include "stone/base/uncopyable.h"
#include "stone/threading/atomic/atomic.h"
#include "stone/threading/this_thread.h"

// GLOBAL_NOLINT(runtime/references)
namespace apsara {
namespace stone {

template<typename T>
class ScopedLocker : private Uncopyable
{
public:
    typedef T LockType;

    explicit ScopedLocker(T* lock) : mLock(lock)
    {
        mLock->Lock();
    }
    explicit ScopedLocker(T& lock) : mLock(&lock)
    {
        mLock->Lock();
    }
    ~ScopedLocker()
    {
        mLock->Unlock();
    }

private:
    T* mLock;
};

template<>
class ScopedLocker<pthread_mutex_t> : private Uncopyable
{
public:
    typedef pthread_mutex_t T;
    typedef T LockType;

    explicit ScopedLocker(T* lock) : mLock(lock)
    {
        CheckPthreadError(pthread_mutex_lock(mLock));
    }
    explicit ScopedLocker(T& lock) : mLock(&lock)
    {
        CheckPthreadError(pthread_mutex_lock(mLock));
    }
    ~ScopedLocker()
    {
        CheckPthreadError(pthread_mutex_unlock(mLock));
    }

private:
    pthread_mutex_t *mLock;
};

template<typename T>
class ScopedUnlocker : private Uncopyable
{
public:
    typedef T LockType;

    explicit ScopedUnlocker(T* lock) : mLock(lock)
    {
        mLock->Unlock();
    }
    explicit ScopedUnlocker(T& lock) : mLock(&lock)
    {
        mLock->Unlock();
    }
    ~ScopedUnlocker()
    {
        mLock->Lock();
    }

private:
    T* mLock;
};

class NullLock : private Uncopyable
{
public:
    typedef ScopedLocker<NullLock> Locker;
    typedef ScopedUnlocker<NullLock> Unlocker;
    NullLock() : mLocked(false) {}
    ~NullLock() {}

    void Lock()
    {
        mLocked = true;
    }
    void Unlock()
    {
        mLocked = false;
    }
    bool IsLocked() const
    {
        return mLocked;
    }

private:
    bool mLocked;
};

class SpinLock : private Uncopyable
{
public:
    typedef ScopedLocker<SpinLock> Locker;
    typedef ScopedUnlocker<SpinLock> Unlocker;
    SpinLock()
    {
        CheckPthreadError(pthread_spin_init(&mSpin, PTHREAD_PROCESS_PRIVATE));
    }
    ~SpinLock()
    {
        CheckPthreadError(pthread_spin_destroy(&mSpin));
    }
    void Lock()
    {
        CheckPthreadError(pthread_spin_lock(&mSpin));
    }
    void Unlock()
    {
        CheckPthreadError(pthread_spin_unlock(&mSpin));
    }
    bool IsLocked() const
    {
        return mSpin == 0;
    }

private:
    pthread_spinlock_t mSpin;
};

class MutexBase : private Uncopyable
{
protected:
    explicit MutexBase(int type)
    {
        pthread_mutexattr_t attr;
        CheckPthreadError(pthread_mutexattr_init(&attr));
        CheckPthreadError(pthread_mutexattr_settype(&attr, type));
        CheckPthreadError(pthread_mutex_init(&mMutex, &attr));
        CheckPthreadError(pthread_mutexattr_destroy(&attr));
    }

public:
    virtual ~MutexBase()
    {
        CheckPthreadError(pthread_mutex_destroy(&mMutex));
    }
    void Lock()
    {
        CheckPthreadError(pthread_mutex_lock(&mMutex));
    }
    void Unlock()
    {
        CheckPthreadError(pthread_mutex_unlock(&mMutex));
    }
    bool IsLocked() const
    {
        return mMutex.__data.__lock > 0;
    }
    pthread_mutex_t* NativeLock()
    {
        return &mMutex;
    }

protected:
    pthread_mutex_t mMutex;
};

class SimpleMutex : public MutexBase
{
public:
    typedef ScopedLocker<SimpleMutex> Locker;
    typedef ScopedUnlocker<SimpleMutex> Unlocker;
    SimpleMutex() : MutexBase(PTHREAD_MUTEX_NORMAL) {}
};

class RestrictMutex : public MutexBase
{
public:
    typedef ScopedLocker<RestrictMutex> Locker;
    typedef ScopedUnlocker<RestrictMutex> Unlocker;
    RestrictMutex() : MutexBase(PTHREAD_MUTEX_ERRORCHECK) {}
};

class RecursiveMutex : public MutexBase
{
public:
    typedef ScopedLocker<RecursiveMutex> Locker;
    typedef ScopedUnlocker<RecursiveMutex> Unlocker;
    RecursiveMutex() : MutexBase(PTHREAD_MUTEX_RECURSIVE) {}
};

class AdaptiveMutex : public MutexBase
{
public:
    typedef ScopedLocker<AdaptiveMutex> Locker;
    typedef ScopedUnlocker<AdaptiveMutex> Unlocker;
    AdaptiveMutex() : MutexBase(PTHREAD_MUTEX_ADAPTIVE_NP) {}
};

// read write lock
template <typename T>
class ScopedReaderLocker : private Uncopyable
{
public:
    typedef T LockType;

    explicit ScopedReaderLocker(T* lock) : mLock(lock)
    {
        mLock->ReadLock();
    }
    explicit ScopedReaderLocker(T& lock) : mLock(&lock)
    {
        mLock->ReadLock();
    }
    ~ScopedReaderLocker()
    {
        mLock->ReadUnlock();
    }
private:
    T* mLock;
};

template <typename T>
class ScopedWriterLocker : private Uncopyable
{
public:
    typedef T LockType;

    explicit ScopedWriterLocker(T* lock) : mLock(lock)
    {
        mLock->WriteLock();
    }
    explicit ScopedWriterLocker(T& lock) : mLock(&lock)
    {
        mLock->WriteLock();
    }
    ~ScopedWriterLocker()
    {
        mLock->WriteUnlock();
    }
private:
    T* mLock;
};

class RWLock : private Uncopyable
{
public:
    typedef ScopedReaderLocker<RWLock> ReaderLocker;
    typedef ScopedWriterLocker<RWLock> WriterLocker;

    enum Mode
    {
        kModePreferReader = PTHREAD_RWLOCK_PREFER_READER_NP,
        kModePreferWriter = PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP,
        kModeDefault = PTHREAD_RWLOCK_DEFAULT_NP,
        PREFER_READER = kModePreferReader,
        PREFER_WRITER = kModePreferWriter
    };

    explicit RWLock(Mode mode = kModeDefault)
    {
        pthread_rwlockattr_t attr;
        CheckPthreadError(pthread_rwlockattr_init(&attr));
        CheckPthreadError(pthread_rwlockattr_setkind_np(&attr, mode));
        CheckPthreadError(pthread_rwlock_init(&mLock, &attr));
        CheckPthreadError(pthread_rwlockattr_destroy(&attr));
    }
    ~RWLock()
    {
        CheckPthreadError(pthread_rwlock_destroy(&mLock));
    }

    void ReadLock()
    {
        CheckPthreadError(pthread_rwlock_rdlock(&mLock));
    }
    void WriteLock()
    {
        CheckPthreadError(pthread_rwlock_wrlock(&mLock));
    }
    bool TryReadLock()
    {
        return CheckPthreadTryLockError(pthread_rwlock_tryrdlock(&mLock));
    }
    bool TryWriteLock()
    {
        return CheckPthreadTryLockError(pthread_rwlock_trywrlock(&mLock));
    }
    void ReadUnlock()
    {
        Unlock();
    }
    void WriteUnlock()
    {
        Unlock();
    }
    void Unlock()
    {
        CheckPthreadError(pthread_rwlock_unlock(&mLock));
    }

private:
    pthread_rwlock_t mLock;
};

// This is a light spin read-write lock, the idea is from the linux kernal.
// It's read perfered, and it's a spin lock! only use it if you are sure in
// you cases, it could lock success quickly, and the punishment is acceptable.
// for detail information, please search 'read_lock' or 'write_lock' in
// linux kernal source code (/include/linux/rwlock.h).
class SpinRWLock : private Uncopyable
{
public:
    typedef ScopedReaderLocker<SpinRWLock> ReaderLocker;
    typedef ScopedWriterLocker<SpinRWLock> WriterLocker;

    SpinRWLock()
    {
        AtomicSet<int>(&mQuota, kInitQuota);
    }
    ~SpinRWLock() {}
    void ReadLock()
    {
        while (!TryReadLock()) {
            ThisThread::Yield();
        }
    }
    void WriteLock()
    {
        while (!TryWriteLock()) {
            ThisThread::Yield();
        }
    }
    bool TryReadLock()
    {
        if (AtomicSub<int>(&mQuota, kRead) >= 0) {
            return true;
        } else {
            AtomicAdd<int>(&mQuota, kRead);
            return false;
        }
    }
    bool TryWriteLock()
    {
        if (AtomicSub<int>(&mQuota, kWrite) >= 0) {
            return true;
        } else {
            AtomicAdd<int>(&mQuota, kWrite);
            return false;
        }
    }
    void ReadUnlock()
    {
        AtomicAdd<int>(&mQuota, kRead);
    }
    void WriteUnlock()
    {
        AtomicAdd<int>(&mQuota, kWrite);
    }

private:
    enum {
        kInitQuota = 0x01000000,
        kWrite = 0x01000000,
        kRead = 0x00000001,
    };
    int mQuota;
};

} // namespace stone
} // namespace apsara

#ifndef NO_STONE_SCOPED_LOCK_MACRO

template<typename Locker>
struct ScopedLockerHelper
{
    Locker locker;
    bool done;

    explicit ScopedLockerHelper(typename Locker::LockType* lock)
      : locker(lock), done(false)
    {
    }
};

#define STONE_SCOPED_LOCK_X(lock, x) \
    /* Please make sure that you have not written */ \
    /* STONE_SCOPED_xxLOCK(lock); {...} */ \
    for (ScopedLockerHelper<x> locker(&lock); \
         !locker.done; \
         locker.done = true) \
        if (true) \

#define STONE_SCOPED_LOCK(lock) \
        STONE_SCOPED_LOCK_X(lock, stone::ScopedLocker<typeof(lock)>)

#define STONE_SCOPED_RLOCK(lock) \
        STONE_SCOPED_LOCK_X(lock, stone::ScopedReaderLocker<typeof(lock)>)

#define STONE_SCOPED_WLOCK(lock) \
        STONE_SCOPED_LOCK_X(lock, stone::ScopedWriterLocker<typeof(lock)>)

#endif // NO_STONE_SCOPED_LOCK_MACRO

#endif // STONE_THREADING_SYNC_LOCK_H
