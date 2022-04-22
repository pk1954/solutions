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
	wstring      m_wstrUnit      {};    // the text to be displayed right of the edit box
	wstring      m_wstrTitle     {};    // the title bar text of the dialog
	float        m_fValue        {};    // the value to be edited in the dialog

	void startTimer(milliseconds const);
	void deleteTimer();
	void trigger()
	{
		Trigger();
		m_bDirty = false;
	}
	void OnInitDlg(HWND const, WPARAM const, LPARAM const) final;
	bool OnOK     (HWND const)                             final; 

	static void CALLBACK TimerProc(void * const, bool const);
};
