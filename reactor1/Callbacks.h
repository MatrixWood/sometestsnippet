#ifndef NET_CALLBACKS_H_
#define NET_CALLBACKS_H_

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

namespace net {
  using TimerCallback = boost::function<void()>;
}

#endif