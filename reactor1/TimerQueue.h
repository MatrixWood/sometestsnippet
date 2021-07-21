#ifndef NET_TIMERQUEUE_H_
#define NET_TIMERQUEUE_H_

#include <set>
#include <vector>

#include <boost/noncopyable.hpp>

#include "Timestamp.h"
#include "Mutex.h"
#include "Callbacks.h"
#include "Channel.h"

namespace net {
  class EventLoop;
  class Timer;
  class TimerId;

  class TimerQueue : boost::noncopyable {
   public:
    TimerQueue(EventLoop* loop);
    ~TimerQueue();

    TimerId addTimer(const TimerCallback& cb,
                     Timestamp when,
                     double interval);
    
   private:
    using Entry = std::pair<Timestamp, Timer*>;
    using TimerList = std::set<Entry>;

    void handleRead();
    std::vector<Entry> getExpired(Timestamp now);
    void reset(const std::vector<Entry>& expired, Timestamp now);

    bool insert(Timer* timer);

    EventLoop* loop_;
    const int timerfd_;
    Channel timerfdChannel_;
    TimerList timers_;
  };
}


#endif