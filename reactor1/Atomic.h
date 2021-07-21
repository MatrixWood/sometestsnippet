#ifndef BASE_ATOMIC_H_
#define BASE_ATOMIC_H_

#include <boost/noncopyable.hpp>
#include <stdint.h>

namespace net {
  namespace detail {
    template <typename T>
    class AtomicIntegerT : boost::noncopyable {
     public:
      AtomicIntegerT() : value_(0) {}

      T get() const { return __sync_val_compare_and_swap(const_cast<volatile T*>(&value_), 0, 0); }
      T getAndAdd(T x) { return __sync_fetch_and_add(&value_, x); }
      T addAndGet(T x) { return __sync_add_and_fetch(&value_, x); }
      T incrementAndGet() { return __sync_add_and_fetch(&value_, 1); }
      void add(T x) { __sync_fetch_and_add(&value_, x); }
      void increment() { __sync_add_and_fetch(&value_, 1); }
      void decrement() { __sync_fetch_and_add(&value_, -1); }
      T getAndSet(T newValue) { return __sync_lock_test_and_set(&value_, newValue); }

     private:
      volatile T value_;
    };
  }

  typedef detail::AtomicIntegerT<int32_t> AtomicInt32;
  typedef detail::AtomicIntegerT<int64_t> AtomicInt64;
}

#endif