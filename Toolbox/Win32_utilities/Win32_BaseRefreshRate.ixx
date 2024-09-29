// Win32_BaseRefreshRate.ixx
//
// Win32_utilities

export module BaseRefreshRate;

import std;
import WinBasics;
import Win32_Util;
import TimerQueueTimer;

using std::wstring;
using std::chrono::milliseconds;
using namespace std::chrono;

export class BaseRefreshRate: public TimerQueueTimer
{
public:

	virtual ~BaseRefreshRate() {};

	virtual void Trigger(bool const = false) = 0;

	void Notify(bool const);

	void RefreshRateDialog(HWND const);

private:
	bool    m_bDirty    { true };
	float   m_fValue    { };    // the value to be edited in the dialog
	wstring m_wstrValue { };    // m_fValue as text
	wstring m_wstrUnit  { };    // the text to be displayed right of the edit box
	wstring m_wstrTitle { };    // the title bar text of the dialog

	void TimerProc() final;

	void startTimer(milliseconds const);
	void deleteTimer();
	void trigger(bool const);

	static void __stdcall TimerProc(void * const, bool const);
};
