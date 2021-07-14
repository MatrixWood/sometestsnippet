#include <pthread.h>
#include <unistd.h>
#include <iostream>

int resourceID = 0;
pthread_rwlock_t myrwlock;

void* read_thread(void* param) {
    while (true) {
        pthread_rwlock_rdlock(&myrwlock);
        std::cout << "read thread ID: " << pthread_self() << ", resourceID: " << resourceID << std::endl;
        sleep(1);
        pthread_rwlock_unlock(&myrwlock);
    }
    return nullptr;
}

void* write_thread(void* param) {
    while (true) {
        pthread_rwlock_wrlock(&myrwlock);
        ++resourceID;
        std::cout << "write thread ID: " << pthread_self() << ", resourceID: " << resourceID << std::endl;

        sleep(1);
        pthread_rwlock_unlock(&myrwlock);
    }
    return nullptr;
}

int main() {
    pthread_rwlockattr_t attr;
    pthread_rwlockattr_init(&attr);
    pthread_rwlockattr_setkind_np(&attr, PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP);
    pthread_rwlock_init(&myrwlock, &attr);
    pthread_t readThreadID[5];
    for (int i = 0; i < 5; ++i) {
        pthread_create(&readThreadID[i], nullptr, read_thread, nullptr);
    }
    pthread_t writeThreadID;
    pthread_create(&writeThreadID, nullptr, write_thread, nullptr);

    pthread_join(writeThreadID, nullptr);

    for (int i = 0; i < 5; ++i) {
        pthread_join(readThreadID[i], nullptr);
    }
    pthread_rwlock_destroy(&myrwlock);

    return 0;
}