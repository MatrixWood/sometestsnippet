#include <pthread.h>
#include <stdio.h>
#include <errno.h>

int main() {
    pthread_mutex_t mymutex;
    pthread_mutex_init(&mymutex, nullptr);
    int ret = pthread_mutex_lock(&mymutex);

    ret = pthread_mutex_destroy(&mymutex);
    if (ret != 0) {
        if (ret == EBUSY) {
            printf("EBUSY\n");
        }
        printf("Failed to destroy mutex.\n");
    }

    ret = pthread_mutex_unlock(&mymutex);
    ret = pthread_mutex_destroy(&mymutex);
    if (ret == 0)
        printf("Succeeded to destroy mutex.\n");
    return 0;
}