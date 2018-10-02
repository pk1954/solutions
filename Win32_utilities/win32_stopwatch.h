// win32_stopwatch.h
//

#pragma once

#include <string>
#include <functional>
#include "win32_hiResTimer.h"

using namespace std;

class Stopwatch
{
public:
	void Start( );
	void Stop( wstring const wstr );

void Clock
( 
	wstring const wstr, function<void( )> code
)
{
	Start( );
	code();
	Stop( wstr );
};

private:
	HiResTimer m_hrtimer;

    static int m_iLevel;
};
