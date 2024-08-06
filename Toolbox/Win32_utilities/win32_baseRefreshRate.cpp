// Win32_RefreshRate.cpp
//
// Toolbox\Win32_utilities

module;

#include <Windows.h>

module BaseRefreshRate;

import std;
import Win32_Util_Resource;
import Win32_Util;
import EditLineBox;
import TimerQueueTimer;

using std::wstring;
using std::to_wstring;
using namespace std::chrono;

void BaseRefreshRate::RefreshRateDialog(HWND const hwndParent)
{
	static float MIN_REFRESH_RATE { 20.0f };

	m_fValue = static_cast<float>(GetRefreshRate().count());
	wstring wstrValue = to_wstring(m_fValue);
	EditLineBox dlgBox(wstrValue, L"Refresh Rate", L"ms");
	if (dlgBox.Show(hwndParent) && ::Evaluate(wstrValue, m_fValue))
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

void BaseRefreshRate::Notify(bool const bImmediately)
{
	m_bDirty = true;
	if (bImmediately || (GetRefreshRate() == 0ms))
		trigger(bImmediately);
}

void BaseRefreshRate::trigger(bool const bImmediately)
{
	Trigger(bImmediately);
	m_bDirty = false;
}

void BaseRefreshRate::TimerProc()
{
	if (m_bDirty)
		trigger(false);
}
