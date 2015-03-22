#include "Mutex.h"

Mutex::Mutex()
{
#ifdef _WIN32
	InitializeCriticalSection(&m_cs);
#else
	pthread_mutex_init(&m_mutex, NULL);
#endif
}

Mutex::~Mutex()
{
#ifdef _WIN32
	DeleteCriticalSection(&m_cs);
#else
	pthread_mutex_destroy(&m_mutex);
#endif
}

void Mutex::lock()
{
#ifdef _WIN32
	EnterCriticalSection(&m_cs);
#else
	pthread_mutex_lock(&m_mutex);
#endif
}

void Mutex::unlock()
{
#ifdef _WIN32
	LeaveCriticalSection(&m_cs);
#else
	pthread_mutex_unlock(&m_mutex);
#endif
}
