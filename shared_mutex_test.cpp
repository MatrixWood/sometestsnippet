#define READER_THREAD_COUNT 8
#define LOOP_COUNT 5000000

#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <thread>

class shared_mutex_counter {
public:
    shared_mutex_counter() = default;
    ~shared_mutex_counter() = default;

    unsigned int get() const {
        std::shared_lock<std::shared_mutex> lock(m_mutex);
        return m_value;
    }

    void increment() {
        std::unique_lock<std::shared_mutex> lock(m_mutex);
        m_value++;
    }

    void reset() {
        std::unique_lock<std::shared_mutex> lock(m_mutex);
        m_value = 0;
    }

private:
    mutable std::shared_mutex m_mutex;
    unsigned int m_value = 0;
};

class mutex_counter {
public:
    mutex_counter() = default;
    ~mutex_counter() = default;

    unsigned int get() const {
        std::unique_lock<std::mutex> lk(m_mutex);
        return m_value;
    }

    void increment() {
        std::unique_lock<std::mutex> lk(m_mutex);
        m_value++;
    }

private:
    mutable std::mutex m_mutex;
    unsigned int m_value = 0;
};

void test_shared_mutex() {
    shared_mutex_counter counter;
    int temp;

    auto writer = [&counter] () {
        for (int i = 0; i < LOOP_COUNT; ++i) {
            counter.increment();
        }
    };

    auto reader = [&counter, &temp] () {
        for (int i = 0; i < LOOP_COUNT; ++i) {
            temp = counter.get();
        }
    };

    std::thread** tarray = new std::thread * [READER_THREAD_COUNT];
    clock_t start = clock();
    for (int i = 0; i < READER_THREAD_COUNT; ++i) {
        tarray[i] = new std::thread(reader);
    }

    std::thread tw(writer);
    for (int i = 0; i < READER_THREAD_COUNT; ++i) {
        tarray[i]->join();
    }

    tw.join();

    clock_t end = clock();
    std::cout << "[test_shared_mutex]\n";
    std::cout << "thread count: " << READER_THREAD_COUNT << "\n";
    std::cout << "result: " << counter.get() << " cost: " << end - start << "ms temp: " << temp << "\n";
}

int main() {
    test_shared_mutex();
    return 0;
}