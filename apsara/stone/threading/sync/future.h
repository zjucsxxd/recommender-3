// Copyright (c) 2013, Alibaba Inc.
// All right reserved.
//
// Author: TANG Ke <ke.tang@aliyun-inc.com>
// Created: 2013/08/20
// Description:

#ifndef STONE_THREADING_SYNC_FUTURE_H
#define STONE_THREADING_SYNC_FUTURE_H

#include <stddef.h>
#include <vector>
#include "stone/base/closure.h"
#include "stone/threading/sync/cond.h"
#include "stone/threading/sync/lock.h"

namespace apsara {
namespace stone {

class Future;
class FutureGroup;

class FutureCompletionCallback {
public:
    virtual ~FutureCompletionCallback() = 0;
    virtual void OnFutureComplete(Future* future) = 0;
};

class Future {
    friend class FutureGroup;

public:
    Future();
    virtual ~Future();

    int Ref();
    int Unref();

    virtual bool Wait(void** pstate = NULL, int timeout = -1) const;
    virtual bool IsCompleted() const;
    virtual void* GetState() const;
    virtual bool Complete(void* state = NULL);
    virtual void Reset();
    virtual bool SetCompletionCallback(FutureCompletionCallback* callback);

    static Future* Create();

private:
    enum State {
        FS_PENDING,
        FS_COMPLETING,
        FS_COMPLETED
    };
    State volatile mState;

    int volatile mRefCount;

    void* volatile mTag;
    mutable int volatile mWaitCount;

    Future* volatile mPrev;
    Future* volatile mNext;

    mutable SpinLock mStateLock;
    mutable SimpleMutex mWaitLock;
    mutable ConditionVariable mWaitCond;

    FutureCompletionCallback* volatile mCompletionCallback;
};

class FuturePtr {
public:
    FuturePtr() {
        mFuture = NULL;
    }

    explicit FuturePtr(Future* future) {
        mFuture = future;
    }

    FuturePtr(const FuturePtr& ptr) {
        Future* future = ptr.mFuture;
        mFuture = future;
        if (future != NULL) {
            future->Ref();
        }
    }

    ~FuturePtr() {
        Reset(NULL);
    }

    FuturePtr& operator=(const FuturePtr& ptr) {
        Future* future = ptr.mFuture;
        if (mFuture == future) {
            return *this;
        }

        Reset(future);
        if (future != NULL) {
            future->Ref();
        }

        return *this;
    }

    void Reset(Future* future) {
        if (mFuture != NULL) {
            mFuture->Unref();
        }
        mFuture = future;
    }

    Future* operator->() const {
        return mFuture;
    }

    operator bool() const {
        return mFuture != NULL;
    }

    Future* Release() {
        Future* future = mFuture;
        mFuture = NULL;
        return future;
    }

    Future* Get() const {
        return mFuture;
    }

private:
    Future* mFuture;
};

class FutureGroup : public FutureCompletionCallback
{
public:
    FutureGroup();
    ~FutureGroup();

    void Add(Future* future);
    int Wait(Future** futures, int size, int timeout = -1);
    int Size() const;
    int CompletionSize() const;
    int PendingSize() const;
    void OnFutureComplete(Future* future);
    int RemovePendings(std::vector<Future*>* futures);

    template<typename T>
    bool WaitOne(T** p, int timeout = -1);

private:
    int mSize;
    int mCompletionSize;

    struct FutureList {
        Future mHead;
        Future mTail;

        bool Empty() {
            return mHead.mNext == &mTail;
        }

        void Reset() {
            mHead.mPrev = NULL;
            mHead.mNext = &mTail;
            mTail.mPrev = &mHead;
            mTail.mNext = NULL;
        }

        void PushBack(Future* future) {
            future->mPrev = mTail.mPrev;
            future->mNext = &mTail;
            mTail.mPrev->mNext = future;
            mTail.mPrev = future;
        }

        Future* RemoveFront() {
            Future* future = NULL;
            if (!Empty()) {
                future = mHead.mNext;
                mHead.mNext = future->mNext;
                future->mNext->mPrev = &mHead;
                future->mPrev = NULL;
                future->mNext = NULL;
            }
            return future;
        }
    } mPending, mCompletion;

    SpinLock mQueueLock;
    SimpleMutex mWaitLock;
    ConditionVariable mWaitCond;
};

template<typename T>
bool FutureGroup::WaitOne(T** p, int timeout) {
    Future** future = reinterpret_cast<Future**>(static_cast<void*>(p));
    int cnt = FutureGroup::Wait(future, 1, timeout);
    if (cnt == 1) {
        return true;
    }

    return false;
}

class FutureClass : public Future, public FutureCompletionCallback {
public:
    explicit FutureClass(Future* future = new Future()) {
        assert(future != NULL);
        mFuture = future;
        mCompletionCallback = NULL;
    }

    ~FutureClass() {
        mFuture->Unref();
        mFuture = NULL;
    }

    /* override */ bool Wait(void** pstate, int timeout) const {
        return mFuture->Wait(pstate, timeout);
    }

    /* override */ bool IsCompleted() const {
        return mFuture->IsCompleted();
    }

    /* override */ void* GetState() const {
        return mFuture->GetState();
    }

    /* override */ bool Complete(void* state = NULL) {
        return mFuture->Complete(state);
    }

    /* override */ void Reset() {
        mFuture->Reset();
    }

    /* override */ bool SetCompletionCallback(
                            FutureCompletionCallback* callback) {
        mCompletionCallback = callback;
        if (mFuture->SetCompletionCallback(this)) {
            return true;
        }
        mCompletionCallback = NULL;

        return false;
    }

    /* override */ void OnFutureComplete(Future* future) {
        if (mCompletionCallback != NULL) {
            mCompletionCallback->OnFutureComplete(this);
        }
    }

protected:
    Future* mFuture;
    bool mOwnFuture;
    FutureCompletionCallback* volatile mCompletionCallback;
};

template<typename _A1>
class FutureClosure : public FutureClass,
                      public apsara::stone::Closure<void, _A1>
{
public:
    explicit FutureClosure()
      : FutureClass()
    {}

    ~FutureClosure() {}

    /* override */ void Run(_A1 a1) {
        mA1 = a1;
        Complete();
    }

    void Run(apsara::stone::Closure<void, _A1>* closure) {
        closure->Run();
    }

    /* override */ bool IsAutoDelete() const {
        return false;
    }

    const _A1& A1() const {
        return mA1;
    }

private:
    _A1 mA1;
};

template<>
class FutureClosure<void> : public FutureClass,
                            public apsara::stone::Closure<void>
{
public:
    explicit FutureClosure()
      : FutureClass(), mClosure(NULL)
    {}

    explicit FutureClosure(apsara::stone::Closure<void>* closure)
        : FutureClass(), mClosure(closure)
    {
    }

    ~FutureClosure()
    {
        if (mClosure != NULL && mClosure->IsAutoDelete()) {
            delete mClosure;
            mClosure = NULL;
        }
    }

    /* override */ void Run() {
        if (mClosure != NULL) {
            mClosure->Run();
            mClosure = NULL;
        }
        Complete();
    }

    void Run(apsara::stone::Closure<void>* closure) {
        closure->Run();
    }

    /* override */ bool IsAutoDelete() const {
        return false;
    }

private:
    apsara::stone::Closure<void>* mClosure;
};

} // namespace stone
} // namespace apsara

#endif // STONE_THREADING_SYNC_FUTURE_H
