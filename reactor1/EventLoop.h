#ifndef NET_EVENTLOOP_H_
#define NET_EVENTLOOP_H_

#include "Timestamp.h"
#include "Thread.h"
#include "Callbacks.h"
#include "TimerId.h"

#include <boost/scoped_ptr.hpp>
#include <vector>

namespace net {
  class Channel;
  class Poller;
  class TimerQueue;

  class EventLoop : boost::noncopyable {
   public:
    EventLoop();
    ~EventLoop();

    void loop();
    void quit();

    Timestamp pollReturnTime() const { return pollReturnTime_; }

    TimerId runAt(const Timestamp& time, const TimerCallback& cb);

    TimerId runAfter(double delay, const TimerCallback& cb);

    TimerId runEvery(double interval, const TimerCallback& cb);

    void updateChannel(Channel* channel);
    
    void assertInLoopThread() {
      if (!isInLoopThread()) {
        abortNotInLoopThread();
      }
    }

    bool isInLoopThread() const { return threadId_ == CurrentThread::tid(); }

   private:
    void abortNotInLoopThread();

    using ChannelList = std::vector<Channel*>;

    bool looping_;
    bool quit_;
    const pid_t threadId_;
    Timestamp pollReturnTime_;
    boost::scoped_ptr<Poller> poller_;
    boost::scoped_ptr<TimerQueue> timerQueue_;
    ChannelList activeChannels_;
  };
}


#endif