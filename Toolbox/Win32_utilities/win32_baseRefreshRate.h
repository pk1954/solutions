// win32_baseRefreshRate.h : 
//
// Win32_utilities

#pragma once

#include <chrono>
#include "win32_util.h"
#include "win32_stdDialogBox.h"

using std::chrono::milliseconds;
using namespace std::chrono_literals;

class BaseRefreshRate : public StdDialogBox
{
public:

	BaseRefreshRate();
	virtual ~BaseRefreshRate();

	virtual void Trigger() = 0;

	void SetRefreshRate(milliseconds const);
	milliseconds GetRefreshRate() const;

	void Notify(bool const bImmediately)
	{
		m_bDirty = true;
		if (bImmediately || (m_msRefreshRate == 0ms))
			trigger();
	}

	void RefreshRateDialog(HWND const);

private:
	HANDLE       m_hTimer        { nullptr };
	milliseconds m_msRefreshRate { 0ms };
	bool         m_bDirty        { true };

	void startTimer(milliseconds const);
	void deleteTimer();
	void trigger()
	{
		Trigger();
		m_bDirty = false;
	}

	static void CALLBACK TimerProc(void * const, bool const);
};
