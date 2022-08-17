// Win32_Stopwatch.ixx
//
// Toolbox/win32_utilities

module;

#include <string>
#include <functional>

export module Stopwatch;

import HiResTimer;

using std::wstring;
using std::function;

export class Stopwatch
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
