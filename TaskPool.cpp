#include "TaskPool.h"

TaskPool::TaskPool() : m_bRunning(false)
{}

TaskPool::~TaskPool() {
    removeAllTasks();
}

void TaskPool::init(int threadNum) {
    if (threadNum <= 0)
        threadNum = 5;
    
    m_bRunning = true;

    for (int i = 0; i < threadNum; ++i) {
        std::shared_ptr<std::thread> spThread;
        spThread.reset(new std::thread(std::bind(&TaskPool::threadFunc, this)));
        m_threads.push_back(spThread);
    }
}

void TaskPool::threadFunc() {
    std::shared_ptr<Task> spTask;
    while (true) {
        std::unique_lock<std::mutex> guard(m_mutexList);
        while (m_taskList.empty()) {
            if (!m_bRunning) break;
            m_cv.wait(guard);
        }

        
    }
}