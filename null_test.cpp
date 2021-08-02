#include <iostream>

int main() {
  int* ptr = nullptr;
  if (ptr) {
    std::cout << "1" << std::endl;
  } else {
    std::cout << "2" << std::endl;
  }
}