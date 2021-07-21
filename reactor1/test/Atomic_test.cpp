#include "../Atomic.h"

#include <iostream>

int main() {
  net::AtomicInt64 a0;
  std::cout << a0.get() << std::endl;
  std::cout << a0.getAndAdd(2) << std::endl;
  std::cout << a0.get() << std::endl;
  std::cout << a0.addAndGet(4) << std::endl;
}