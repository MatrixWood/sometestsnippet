#include "Thread.h"

#include <boost/weak_ptr.hpp>

#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>

#include <sys/prctl.h>
#include <linux/unistd.h>

namespace base {

namespace CurrentThread {
  __thread const char* t_threadName = "unnamedThread";
}
}

namespace {
  __thread pid_t t_cachedTid = 0;

#if !__GLIBC_PREREQ(2, 30)
  pid_t gettid() {
    return static_cast<pid_t>(::syscall(SYS_gettid));
  }
#endif

  void afterFork() {
    t_cachedTid = gettid();
    base::CurrentThread::t_threadName = "main";
  }

  class ThreadNameInitializer {
   public:
    ThreadNameInitializer() {
      base::CurrentThread::t_threadName = "main";
      pthread_atfork(nullptr, nullptr, &afterFork);
    }
  };

  ThreadNameInitializer init;

  struct ThreadData {
    using ThreadFunc = base::Thread::ThreadFunc;
    ThreadFunc func_;
    std::string name_;
    boost::weak_ptr<pid_t> wkTid_;

    ThreadData(const ThreadFunc& func,
               const std::string& name,
               const boost::shared_ptr<pid_t>& tid)
      : func_(func),
        name_(name),
        wkTid_(tid)
    { }

    void runInThread() {
      pid_t tid = base::CurrentThread::tid();
      boost::shared_ptr<pid_t> ptid = wkTid_.lock();
      if (ptid) {
        *ptid = tid;
        ptid.reset();
      }
      if (!name_.empty()) {
        base::CurrentThread::t_threadName = name_.c_str();
      }
      ::prctl(PR_SET_NAME, base::CurrentThread::t_threadName);
      func_();
      base::CurrentThread::t_threadName = "finished";
    }
  };

  void* startThread(void* obj) {
    ThreadData* data = static_cast<ThreadData*>(obj);
    data->runInThread();
    delete data;
    return nullptr;
  }
}

using namespace base;

pid_t CurrentThread::tid() {
  if (t_cachedTid == 0) {
    t_cachedTid = gettid();
  }
  return t_cachedTid;
}

const char* CurrentThread::name() {
  return t_threadName;
}

bool CurrentThread::isMainThread() {
  return tid() == ::getpid();
}

AtomicInt32 Thread::numCreated_;

Thread::Thread(const ThreadFunc& func, const std::string& n)
  : started_(false),
    joined_(false),
    pthreadId_(0),
    tid_(new pid_t(0)),
    func_(func),
    name_(n)
{
  numCreated_.increment();
}

Thread::~Thread() {
  if (started_ && !joined_) {
    pthread_detach(pthreadId_);
  }
}

void Thread::start() {
  assert(!started_);
  started_ = true;

  ThreadData* data = new ThreadData(func_, name_, tid_);
  if (pthread_create(&pthreadId_, nullptr, &startThread, data)) {
    started_ = false;
    delete data;
    abort();
  }
}

void Thread::join() {
  assert(started_);
  assert(!joined_);
  joined_ = true;
  pthread_join(pthreadId_, nullptr);
}