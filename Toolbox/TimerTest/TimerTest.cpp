// TimerTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "windows.h"
#include <iostream>
#include <condition_variable>
#include <mutex>

using namespace std;

long lGlobal = 42;

LONGLONG a = 9;
LONGLONG b = 7;
LONGLONG c = 5;

std::mutex                   myMutex;
std::condition_variable      condVar;
std::unique_lock<std::mutex> myLock(myMutex);

int _tmain(int argc, _TCHAR* argv[])
{
	const int COUNT = 10000000L;

	SRWLOCK m_SRWLock { SRWLOCK_INIT };

	HANDLE hMutex = CreateMutex
	( 
		NULL,              // default security attributes
		FALSE,             // initially not owned
		NULL               // unnamed mutex
	);

	ULONGLONG milliSeconds = GetTickCount64();

	for ( int i = 0; i < COUNT; i++ )
	{
		++lGlobal;
	}

	ULONGLONG tara = GetTickCount64() - milliSeconds;
	milliSeconds = GetTickCount64();

	for ( int i = 0; i < COUNT; i++ )
	{
//		AcquireSRWLockExclusive( & m_SRWLock );
//		InterlockedCompareExchangeNoFence64( & a, b, c) ;
//		WaitForSingleObject( hMutex, INFINITE );
		condVar.wait( myLock );
		++lGlobal;
		myLock.unlock( );
		condVar.notify_one( );
//		ReleaseMutex( hMutex );
//		ReleaseSRWLockExclusive( & m_SRWLock ); 
	}

	ULONGLONG brutto = GetTickCount64() - milliSeconds;
	ULONGLONG netto  = brutto - tara;

	cout << "brutto = " << brutto/10 << " ns"  << endl;
	cout << "tara   = " << tara  /10 << " ns"  << endl;
	cout << "netto  = " << netto /10 << " ns" << endl;

	return 0;
}
