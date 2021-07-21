#ifndef NET_TIMER_H_
#define NET_TIMER_H_

#include <boost/noncopyable.hpp>

#include "Timestamp.h"
#include "Callbacks.h"

namespace net {

class Timer : boost::noncopyable {
 public:
  Timer(const TimerCallback& cb, Timestamp when, double interval)
    : callback_(cb),
      expiration_(when),
      interval_(interval),
      repeat_(interval > 0.0)
  { }

  void run() const {
    callback_();
  }

  Timestamp expiration() const { return expiration_; }
  bool repeat() const { return repeat_; }

  void restart(Timestamp now) {
    if (repeat_) {
      expiration_ = addTime(now, interval_);
    } else {
      expiration_ = Timestamp::invalid();
    }
  }

 private:
  const TimerCallback callback_;
  Timestamp expiration_;
  const double interval_;
  const bool repeat_;
};
}

#endif