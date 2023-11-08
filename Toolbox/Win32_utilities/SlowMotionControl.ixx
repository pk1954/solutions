// SlowMotionControl.ixx
//
// Win32_utilities

module;

#include <compare>
#include <Windows.h>

export module NNetWin32:SlowMotionControl;

import Win32_Util_Resource;
import StatusBar;

export class SlowMotionControl
{
public:

	static void Add(StatusBar & statusBar)
	{ 
		statusBar.AddButton(L" Slower ", IDM_SLOWER, BS_PUSHBUTTON); 
		statusBar.AddButton(L" Faster ", IDM_FASTER, BS_PUSHBUTTON); 
	} 
};
