#ifndef NET_POLLER_H_
#define NET_POLLER_H_

#include <map>
#include <vector>

#include "Timestamp.h"
#include "EventLoop.h"

struct pollfd;

namespace net {
  class Channel;

  class Poller : boost::noncopyable {
   public:
    using ChannelList = std::vector<Channel*>;

    Poller(EventLoop* loop);
    ~Poller();

    Timestamp poll(int timeoutMs, ChannelList* activeChannels);

    void updateChannel(Channel* channel);
    void assertInLoopThread() {
      ownerLoop_->assertInLoopThread();
    }

   private:
    void fillActiveChannels(int numChannels,
                            ChannelList* activeChannels) const;
    
    using PollFdList = std::vector<struct pollfd>;
    using ChannelMap = std::map<int, Channel*>;

    EventLoop* ownerLoop_;
    PollFdList pollfds_;
    ChannelMap channels_;
  };
}

#endif