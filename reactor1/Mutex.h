#ifndef BASE_MUTEX_H_
#define BASE_MUTEX_H_

#include "Thread.h"

#include <boost/noncopyable.hpp>
#include <assert.h>
#include <pthread.h>

namespace net {
  class MutexLock : boost::noncopyable {
   public:
    MutexLock() : holder_(0) {
      pthread_mutex_init(&mutex_, nullptr);
    }

    ~MutexLock() {
      assert(holder_ == 0);
      pthread_mutex_destroy(&mutex_);
    }

    bool isLockedByThisThread() {
      return holder_ == CurrentThread::tid();
    }

    void assertLocked() {
      assert(isLockedByThisThread());  
    }

    void lock() {
      pthread_mutex_lock(&mutex_);
      holder_ = CurrentThread::tid();
    }

    void unlock() {
      holder_ = 0;
      pthread_mutex_unlock(&mutex_);
    }

    pthread_mutex_t* getPthreadMutex() {
      return &mutex_;
    }
  
   private:
    pthread_mutex_t mutex_;
    pid_t holder_;
  };

  class MutexLockGuard : boost::noncopyable {
   public:
    explicit MutexLockGuard(MutexLock& mutex) : mutex_(mutex) {
      mutex_.lock();
    }

    ~MutexLockGuard() {
      mutex_.unlock();
    }

   private:
    MutexLock& mutex_;
  };
}

#endif