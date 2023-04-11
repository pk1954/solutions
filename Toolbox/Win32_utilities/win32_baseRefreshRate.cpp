// Win32_RefreshRate.cpp
//
// Toolbox\Win32_utilities

module;

#include <cassert>
#include <string>
#include <chrono>
#include <Windows.h>

module BaseRefreshRate;

import Win32_Util_Resource;
import Win32_Util;
import EditLineBox;

using std::wstring;
using std::to_wstring;
using std::bit_cast;
using std::chrono::milliseconds;
using namespace std::chrono;

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

milliseconds BaseRefreshRate::GetRefreshRate() const
{ 
	return m_msRefreshRate; 
}

void BaseRefreshRate::RefreshRateDialog(HWND const hwndParent)
{
	static float MIN_REFRESH_RATE { 20.0f };

	m_fValue = static_cast<float>(GetRefreshRate().count());
	wstring wstrValue = to_wstring(m_fValue);
	EditLineBox dlgBox(wstrValue, L"Refresh Rate", L"ms");
	if (dlgBox.Show(hwndParent) && Util::Evaluate(wstrValue, m_fValue))
	{
		if (m_fValue < MIN_REFRESH_RATE)
		{
			wstring text { L"Minimum refresh rate is " + to_wstring(MIN_REFRESH_RATE) + L" ms" };
			MessageBox(nullptr, text.c_str(), nullptr, MB_OK);
			m_fValue = MIN_REFRESH_RATE;
		}
		SetRefreshRate(static_cast<milliseconds>(static_cast<long long>(m_fValue)));
	}
}

void BaseRefreshRate::startTimer(milliseconds const msTimer)
{
	auto dwTime = static_cast<DWORD>(msTimer.count());
	(void)CreateTimerQueueTimer
	(
		& m_hTimer,                     // output parameter 
		nullptr,                        // use default timer queue
		(WAITORTIMERCALLBACK)TimerProc, // the timer procedure
		static_cast<void *>(this),      // pointer to this object as parameter to TimerProc
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
	auto const pRefreshRate = bit_cast<BaseRefreshRate *>(lpParam);
	if (pRefreshRate->m_bDirty)
		pRefreshRate->trigger();
}
