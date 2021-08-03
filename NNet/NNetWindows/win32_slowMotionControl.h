// win32_slowMotionControl.h
//
// NNetSimu

#pragma once

#include "Resource.h"
#include "win32_status.h"

class SlowMotionControl
{
public:

	static void Add(StatusBar * const pStatusBar)
	{ 
		pStatusBar->AddButton(L" Slower ", (HMENU)IDM_SLOWER, BS_PUSHBUTTON); 
		pStatusBar->AddButton(L" Faster ", (HMENU)IDM_FASTER, BS_PUSHBUTTON); 
	} 

private:
};
