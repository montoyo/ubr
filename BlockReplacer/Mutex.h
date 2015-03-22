#pragma once
#ifdef _WIN32
#include <Windows.h>
#else
#include <pthread.h>
#endif

class Mutex
{
public:
	Mutex();
	~Mutex();

	void lock();
	void unlock();

private:
#ifdef _WIN32
	CRITICAL_SECTION m_cs;
#else
	pthread_mutex_t m_mutex;
#endif
};
