// Win32_SlowMotionControl.ixx
//
// NNetWindows

module;

#include <compare>
#include <Windows.h>
#include "Resource.h"

export module SlowMotionControl;

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
