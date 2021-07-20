#include "EventLoop.h"
#include "../base/Thread.h"
#include <iostream>

void threadFunc() {
  std::cout << "threadFunc(): pid = " << getpid()
    << ", tid = " << base::CurrentThread::tid() << "\n";
  net::EventLoop loop;
  loop.loop();
}

int main() {
  std::cout << "main(): pid = " << getpid()
    << ", tid = " << base::CurrentThread::tid() << "\n";
  net::EventLoop loop;
  base::Thread thread(threadFunc);
  thread.start();

  loop.loop();
  pthread_exit(nullptr);
}