#pragma once

#include <Windows.h>

class LibMutex 
{
public:
	LibMutex();
	~LibMutex();

	virtual void Lock() const;
	virtual void Unlock() const;

private:
	HANDLE m_mutex;
};

class LibMutexLocker
{
public:
	LibMutexLocker(LibMutex* pMutex);

	~LibMutexLocker();
	void Lock();
	void Unlock();
protected:
private:
	LibMutex* m_pMutex;
};


