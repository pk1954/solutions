// win32_modelWindow.h : 
//
// Win32_utilities

#pragma once

#include "win32_baseWindow.h"
#include "win32_ActionTimer.h"

class ActionTimer;

class ModelWindow : public BaseWindow
{
public:
	static void InitClass(ActionTimer * const pActionTimer)
	{
		m_pDisplayTimer = pActionTimer;
		m_hCrsrArrow    = LoadCursor(nullptr, IDC_ARROW);
		m_hCrsrMove     = LoadCursor(nullptr, IDC_HAND);
		m_hCrsrWait     = LoadCursor(nullptr, IDC_WAIT);
	}

protected:

	inline static ActionTimer * m_pDisplayTimer { nullptr };
	inline static HCURSOR       m_hCrsrMove     { nullptr };
	inline static HCURSOR       m_hCrsrArrow    { nullptr };
	inline static HCURSOR       m_hCrsrWait     { nullptr };
};