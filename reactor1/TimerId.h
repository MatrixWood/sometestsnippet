#ifndef NET_TIMERID_H_
#define NET_TIMERID_H_

#include "copyable.h"

namespace net {
  class Timer;

  class TimerId : public net::copyable {
   public:
    explicit TimerId(Timer* timer)
      : value_(timer)
    {}

   private:
    Timer* value_;
  };
}


#endif