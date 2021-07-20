#ifndef NET_EVENTLOOP_H_
#define NET_EVNETLOOP_H_

#include "../base/Thread.h"

#include <boost/scoped_ptr.hpp>
#include <vector>

namespace net {
class Channel;
class Poller;

class EventLoop : boost::noncopyable {
 public:
  EventLoop();
  ~EventLoop();

  void loop();
  void quit();
  void updateChannel(Channel* channel);
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

  using ChannelList = std::vector<Channel*>;
  bool looping_;
  bool quit_;
  const pid_t threadId_;
  boost::scoped_ptr<Poller> poller_;
  ChannelList activeChannels_;
};

}


#endif