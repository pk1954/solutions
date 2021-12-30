// win32_slowMotionControl.h
//
// NNetSimu

#pragma once

#include "Resource.h"
#include "win32_status.h"

class SlowMotionControl
{
public:

	static void Add(StatusBar & statusBar)
	{ 
		statusBar.AddButton(L" Slower ", IDM_SLOWER, BS_PUSHBUTTON); 
		statusBar.AddButton(L" Faster ", IDM_FASTER, BS_PUSHBUTTON); 
	} 
};
