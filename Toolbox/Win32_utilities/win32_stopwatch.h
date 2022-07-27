// win32_stopwatch.h
//
// Toolbox/win32_utilities

#pragma once

#include <string>
#include <functional>
#include "win32_hiResTimer.h"

using std::wstring;
using std::function;

class Stopwatch
{
public:
	void Start();
	void Stop(wstring const & wstr);

void Clock(wstring const wstr, function<void()> code)
{
	Start();
	code();
	Stop(wstr);
};

private:
	HiResTimer m_hrtimer;

	inline static int m_iLevel { 0 };
};
