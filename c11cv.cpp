#include <thread>
#include <mutex>
#include <condition_variable>
#include <unistd.h>
#include <list>
#include <iostream>
#include <chrono>

class Task {
public:
    Task(int taskID) {
        this->taskID = taskID;
    }

    void doTask() {
        std::cout << "handle a task, taskID: " << taskID << ", threadID: " <<
            std::this_thread::get_id() << std::endl;
    }

private:
    int taskID;
};

std::mutex mymutex;
std::list<Task*> tasks;
std::condition_variable mycv;

void* consumer_thread() {
    Task* pTask = nullptr;
    while (true) {
        std::unique_lock<std::mutex> guard(mymutex);

        while (tasks.empty()) {
            mycv.wait(guard);
        }

        pTask = tasks.front();
        tasks.pop_front();
        if (pTask == nullptr) continue;

        pTask->doTask();
        delete pTask;
        pTask = nullptr;
    }
    return nullptr;
}

void* producer_thread() {
    int taskID = 0;
    Task* pTask = nullptr;
    while (true) {
        pTask = new Task(taskID);
        {
            std::lock_guard<std::mutex> guard(mymutex);
            tasks.push_back(pTask);
            std::cout << "produce a task, taskID: " << taskID << ", threadID: " << std::this_thread::get_id() << std::endl;
        }
        mycv.notify_one();
        taskID++;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    return nullptr;    
}

int main() {

    std::thread** tarray = new std::thread * [5];
    for (int i = 0; i < 5; ++i) {
        tarray[i] = new std::thread(consumer_thread);
    }

    std::thread producer(producer_thread);
    producer.join();
    for (int i = 0; i < 5; ++i) {
        tarray[i]->join();
    }

    for (int i = 0; i < 5; ++i) {
        delete tarray[i];
    }
    delete[] tarray;
    return 0;
}