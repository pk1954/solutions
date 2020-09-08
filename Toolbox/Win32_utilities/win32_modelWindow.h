// win32_modelWindow.h : 
//
// Win32_utilities

#pragma once

#include "win32_baseWindow.h"
#include "win32_actionTimer.h"

class ActionTimer;

class ModelWindow : public BaseWindow
{
public:
	ModelWindow( ) :
		BaseWindow( )
	{ }

	static void InitClass( ActionTimer * const pActionTimer	)
	{
		m_pDisplayTimer = pActionTimer;
		m_hCrsrArrow    = LoadCursor( NULL, IDC_ARROW );
		m_hCrsrMove     = LoadCursor( NULL, IDC_HAND );
	}

protected:

	inline static ActionTimer * m_pDisplayTimer { nullptr };
	inline static HCURSOR       m_hCrsrMove     { nullptr };
	inline static HCURSOR       m_hCrsrArrow    { nullptr };

};