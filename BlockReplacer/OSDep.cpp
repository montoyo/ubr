#include "OSDep.h"
#include <cstring>

#ifdef _WIN32

bool scanDir(const char *tdir, std::vector<std::string> &out)
{
	int len = strlen(tdir);
	if(len > MAX_PATH - 3)
		return false;

	char dir[MAX_PATH];
	memcpy(dir, tdir, len);

	dir[len] = '\\';
	dir[len + 1] = '*';
	dir[len + 2] = 0;

	WIN32_FIND_DATA wfd;
	HANDLE f = FindFirstFile(dir, &wfd);

	if(f == INVALID_HANDLE_VALUE)
		return false;

	do {
		if((wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
			len = strlen(wfd.cFileName) - 4;
			if(len <= 0)
				continue;

			char ext[5];
			memcpy(ext, wfd.cFileName + len, 5);

			for(int i = 0; i < 4; i++)
				ext[i] = tolower(ext[i]);

			if(strcmp(ext, ".mca") == 0)
				out.push_back(wfd.cFileName);
		}
	} while(FindNextFile(f, &wfd) != 0);

	FindClose(f);
	return true;
}

int getCPUCount()
{
	SYSTEM_INFO sis;
	GetSystemInfo(&sis);

	return sis.dwNumberOfProcessors;
}

unsigned long getTimeMs()
{
	return GetTickCount();
}

void waitMs(int time)
{
	Sleep(time);
}

void makeThread(Thread *t, void *func, void *ud)
{
	*t = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) func, ud, 0, NULL);
}

void freeThread(Thread t)
{
	CloseHandle(t);
}

void setThreadCore(Thread t, int core)
{
	SetThreadAffinityMask(t, 1 << core);
}

bool isThreadValid(Thread t)
{
	return (t != INVALID_HANDLE_VALUE);
}

void setThreadInvalid(Thread *t)
{
	*t = INVALID_HANDLE_VALUE;
}

void waitForThread(Thread t)
{
	WaitForSingleObject(t, INFINITE);
}

#else

typedef void * (*LStartRoutine)(void *);

bool scanDir(const char *tdir, std::vector<std::string> &out)
{
	DIR *d = opendir(tdir);

	if(d == NULL)
		return true;

	struct dirent *entry;

	while((entry = readdir(d)) != NULL) {
		if(entry->d_type == DT_REG) {
			int len = strlen(entry->d_name) - 4;
			if(len <= 0)
				continue;

			char ext[5];
			memcpy(ext, entry->d_name + len, 5);

			for(int i = 0; i < 4; i++)
				ext[i] = tolower(ext[i]);

			if(strcmp(ext, ".mca") == 0)
				out.push_back(entry->d_name);
		}
	}

	closedir(d);
	return true;
}

int getCPUCount()
{
	return sysconf(_SC_NPROCESSORS_ONLN);
}

unsigned long getTimeMs()
{
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);

	unsigned long ret = ((unsigned long) ts.tv_nsec) / 1000000; //To milis
	ret += ((unsigned long) ts.tv_sec) * 1000;

	return ret;
}

void waitMs(int time)
{
	usleep(time * 1000);
}

void makeThread(Thread *t, void *func, void *ud)
{
	pthread_create(t, NULL, (LStartRoutine) func, ud);
}

void freeThread(Thread t)
{
}

void setThreadCore(Thread t, int core)
{
	cpu_set_t cpus;
	CPU_ZERO(&cpus);
	CPU_SET(core, &cpus);

	pthread_setaffinity_np(t, sizeof(cpu_set_t), &cpus);
}

bool isThreadValid(Thread t)
{
	return (t != 0);
}

void setThreadInvalid(Thread *t)
{
	*t = 0;
}

void waitForThread(Thread t)
{
	pthread_join(t, NULL);
}

#endif
