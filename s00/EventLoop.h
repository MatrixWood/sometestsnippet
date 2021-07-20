#ifndef NET_EVENTLOOP_H_
#define NET_EVENTLOOP_H_

#include "../base/Thread.h"

namespace net {

class EventLoop : boost::noncopyable {
 public:
  EventLoop();
  ~EventLoop();

  void loop();
  void assertInLoopThread() {
    if (!isInLoopThread()) {
      abortNotInLoopThread();
    }
  }

  bool isInLoopThread() const {
    return threadId_ == base::CurrentThread::tid();
  }

 private:
  void abortNotInLoopThread();

  bool looping_;
  const pid_t threadId_;
};

}

#endif