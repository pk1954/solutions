// win32_stopwatch.h
//

#pragma once

#include <string>
#include <functional>
#include "win32_hiResTimer.h"

class Stopwatch
{
public:
	void Start( );
	void Stop( std::wstring const wstr );

void Clock
( 
	std::wstring const wstr, std::function<void( )> code
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
