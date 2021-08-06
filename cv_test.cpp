#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <list>
#include <semaphore.h>
#include <iostream>

class Task {
public:
    Task(int taskID) {
        this->taskID = taskID;
    }

    void doTask() {
        std::cout << "handle a task, taskID: " << taskID << ", threadID: " << pthread_self() << std::endl;
    }

private:
    int taskID;
};

pthread_mutex_t mymutex;
std::list<Task*> tasks;
pthread_cond_t mycv;

void* consumer_thread(void* param) {
    Task* pTask = nullptr;
    while (true) {
        pthread_mutex_lock(&mymutex);
        while (tasks.empty()) {
            pthread_cond_wait(&mycv, &mymutex);
        }
        pTask = std::move(tasks.front());
        std::cout << "fuck " << std::endl;
        tasks.pop_front();
        pthread_mutex_unlock(&mymutex);
        if (pTask == nullptr)
            continue;
        
        pTask->doTask();
    }
    return nullptr;
}

void* producer_thread(void* param) {
    int taskID = 0;
    Task* pTask;
    while (true) {
        Task* p_tmp(new Task(taskID));
        pTask = std::move(p_tmp);
        pthread_mutex_lock(&mymutex);
        tasks.push_back(pTask);
        std::cout << "produce a task, taskID: " << taskID << ", threadID: " << pthread_self() << std::endl;
        pthread_mutex_unlock(&mymutex);
        pthread_cond_signal(&mycv);
        taskID++;
        sleep(1);
    }
    return nullptr;
}

int main() {
    pthread_mutex_init(&mymutex, nullptr);
    pthread_cond_init(&mycv, nullptr);

    pthread_t consumerThreadID[1];
    for (int i = 0; i < 1; ++i) {
        pthread_create(&consumerThreadID[i], nullptr, consumer_thread, nullptr);
    }
    pthread_t producerThreadID;
    pthread_create(&producerThreadID, nullptr, producer_thread, nullptr);

    pthread_join(producerThreadID, nullptr);

    for (int i = 0; i < 1; ++i) {
        pthread_join(consumerThreadID[i], nullptr);
    }

    pthread_cond_destroy(&mycv);
    pthread_mutex_destroy(&mymutex);

    return 0;
}