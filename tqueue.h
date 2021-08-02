#ifndef TQUEUE_H_
#define TQUEUE_H_

#include <iostream>
#include <memory>

const int MAX_STACK_SIZE = 10000;

template <typename T>
class TQueue {
 public:
  TQueue(const int size = 6);
  ~TQueue();

  void pop();
  void push(T elem);
  bool topBorder();
  bool bottomBorder();
  bool isEmpty();
  bool isFull();

 private:
  std::unique_ptr<T[]> mem_;
  int size_;
  int top_;
  int bottom_;
};

template <typename T>
TQueue<T>::TQueue(const int size)
  : top(-1),
    bottom_(-1)
{
  if ((size < 0) || (size > MAX_STACK_SIZE)) throw "Error";
  size_ = s;
  mem_.reset(new T[size_]);
}

template <typename T>
TQueue<T>::~TQueue()
{
}

template <typename T>
bool TQueue<T>::bottomBorder()
{
  if (bottom_ == -1) return true;
  return false;
}


#endif