#pragma once
#include <vector>
#include <string>

#ifdef _WIN32
#include <Windows.h>

#define THREAD_FUNC(name, param) DWORD WINAPI name(LPVOID param)
#define THREAD_RETVAL 0

typedef HANDLE Thread;

bool scanDir(const char *tdir, std::vector<std::string> &out);
int getCPUCount();
unsigned long getTimeMs();
void waitMs(int time);
void makeThread(Thread *t, void *func, void *ud);
void freeThread(Thread t);
void setThreadCore(Thread t, int core);
bool isThreadValid(Thread t);
void setThreadInvalid(Thread *t);
void waitForThread(Thread t);

#else
#include <unistd.h>
#include <time.h>
#include <dirent.h>
#include <pthread.h>

#define THREAD_FUNC(name, param) void *name(void *param)
#define THREAD_RETVAL NULL

typedef pthread_t Thread;

bool scanDir(const char *tdir, std::vector<std::string> &out);
int getCPUCount();
unsigned long getTimeMs();
void waitMs(int time);
void makeThread(Thread *t, void *func, void *ud);
void freeThread(Thread t);
void setThreadCore(Thread t, int core);
bool isThreadValid(Thread t);
void setThreadInvalid(Thread *t);
void waitForThread(Thread t);

#endif
