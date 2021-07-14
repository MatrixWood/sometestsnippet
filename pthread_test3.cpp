#include <stdio.h>
#include <string.h>
#include <time.h>
#include <thread>

#define TIME_FILENAME "time.txt"

void FileThreadFunc() {
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    char timeStr[32] = {0};
    snprintf(timeStr, 32, "%04d/%02d/%02d %02d:%02d:%02d",
        t->tm_year + 1900,
        t->tm_mon + 1,
        t->tm_mday,
        t->tm_hour,
        t->tm_min,
        t->tm_sec);
    FILE* fp = fopen(TIME_FILENAME, "w");
    if (fp = NULL) {
        printf("Failed to create time.txt\n");
        return ;
    }
    size_t sizeToWrite = strlen(timeStr) + 1;
    size_t ret = fwrite(timeStr, 1, sizeToWrite, fp);
    if (ret != sizeToWrite) {
        printf("Write file error.\n");
    }
    fclose(fp);
}

int main() {
    std::thread t(FileThreadFunc);
    if (t.joinable()) 
        t.join();
    
    FILE* fp = fopen(TIME_FILENAME, "r");
    if (fp == NULL) {
        printf("open file error\n");
        return -1;
    }
    char buf[32] = {0};
    int sizeRead = fread(buf, 1, 32, fp);
    if (sizeRead == 0) {
        printf("read file error.\n");
        fclose(fp);
        return -2;
    }
    printf("current time is: %s\n", buf);
    fclose(fp);
    return 0;
    
}