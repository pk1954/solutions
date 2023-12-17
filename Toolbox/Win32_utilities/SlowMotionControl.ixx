// SlowMotionControl.ixx
//
// Win32_utilities

module;

#include <compare>
#include <Windows.h>

export module SlowMotionControl;

import Win32_Util_Resource;
import StatusBar;

export class SlowMotionControl
{
public:

	static void Add(StatusBar & statusBar)
	{ 
		m_hwndSlower   = statusBar.AddButton(L" Slower ", IDM_SLOWER,    BS_PUSHBUTTON); 
		m_hwndFaster   = statusBar.AddButton(L" Faster ", IDM_FASTER,    BS_PUSHBUTTON); 
		m_hwndMaxSpeed = statusBar.AddButton(L"  Max   ", IDM_MAX_SPEED, BS_PUSHBUTTON); 
	}

	static void Enable(bool const bEnable)
	{
		EnableWindow(m_hwndSlower,   bEnable);
		EnableWindow(m_hwndFaster,   bEnable);
		EnableWindow(m_hwndMaxSpeed, bEnable);
	}

private:
	static inline HWND m_hwndSlower;
	static inline HWND m_hwndFaster;
	static inline HWND m_hwndMaxSpeed;
};
