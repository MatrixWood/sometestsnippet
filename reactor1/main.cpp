#include "EventLoop.h"

#include <iostream>
#include <boost/bind.hpp>

int cnt = 0;
net::EventLoop* g_loop;

void printTid() {
  std::cout << "pid = " << getpid() << ", tid = " << net::CurrentThread::tid() << std::endl;
  std::cout << "now " << net::Timestamp::now().toFormattedString().c_str() << std::endl;
}

void print(const char* msg) {
  std::cout << "msg " << net::Timestamp::now().toFormattedString().c_str()
    << " " << msg << ", cnt = " << cnt << std::endl;
  
  if (++cnt == 20) {
    g_loop->quit();
  }
}

int main() {
  printTid();
  net::EventLoop loop;
  g_loop = &loop;

  print("main");
  loop.runAfter(1, boost::bind(print, "once1"));
  loop.runAfter(1.5, boost::bind(print, "once1.5"));
  loop.runAfter(2.5, boost::bind(print, "once2.5"));
  loop.runAfter(3.5, boost::bind(print, "once3.5"));
  loop.runEvery(2, boost::bind(print, "every2"));
  loop.runEvery(3, boost::bind(print, "every3"));

  loop.loop();
  std::cout << "main loop exits";
  sleep(1);
}