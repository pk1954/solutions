// win32_refreshRate.cpp
//
// Win32_utilities

#include "stdafx.h"
#include "win32_stdDialogBox.h"
#include "win32_baseRefreshRate.h"

using std::wstring;
using std::to_wstring;

BaseRefreshRate::BaseRefreshRate()
{}

BaseRefreshRate::~BaseRefreshRate()
{
	deleteTimer();
	m_hTimer = nullptr;
}

void BaseRefreshRate::SetRefreshRate(milliseconds const msRate) 
{ 
	m_msRefreshRate = msRate; 
	deleteTimer();
	startTimer(msRate);
}

milliseconds BaseRefreshRate::GetRefreshRate()
{ 
	return m_msRefreshRate; 
}

void BaseRefreshRate::RefreshRateDialog(HWND const hwnd)
{
	static double MIN_REFRESH_RATE { 20.0 };

	double dNewValue = StdDialogBox::Show
	(
		hwnd,
		static_cast<float>(GetRefreshRate().count()),
		L"Refresh Rate",
		L"milliseconds"
	);
	if (dNewValue < MIN_REFRESH_RATE)
	{
		wstring text { L"Minimum refresh rate is " + to_wstring(MIN_REFRESH_RATE) + L" ms" };
		MessageBox(nullptr, text.c_str(), NULL, MB_OK);
		dNewValue = MIN_REFRESH_RATE;
	}
	SetRefreshRate(static_cast<milliseconds>(static_cast<long long>(dNewValue)));
}

void BaseRefreshRate::startTimer(milliseconds const msTimer)
{
	DWORD dwTime = static_cast<DWORD>(msTimer.count());
	(void)CreateTimerQueueTimer
	(
		& m_hTimer,                     // output parameter 
		nullptr,                        // use default timer queue
		(WAITORTIMERCALLBACK)TimerProc, // the timer procedure
		static_cast<void *>(this),    // pointer to this object as parameter to TimerProc
		dwTime,                         // timer is signaled the first time after dwTime msecs
		dwTime,                         // timer is signaled periodically every dwTime msecs
		0                               // no flags
	);
	assert(m_hTimer != nullptr);
}

void BaseRefreshRate::deleteTimer()
{
	if (m_hTimer != nullptr)
	{
		(void)DeleteTimerQueueTimer(nullptr, m_hTimer, INVALID_HANDLE_VALUE);
		m_hTimer = nullptr;
	}
}

void CALLBACK BaseRefreshRate::TimerProc(void * const lpParam, bool const TimerOrWaitFired)
{
	BaseRefreshRate * const pRefreshRate = reinterpret_cast<BaseRefreshRate *>(lpParam);
	if (pRefreshRate->m_bDirty)
		pRefreshRate->trigger();
}
