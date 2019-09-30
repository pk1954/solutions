// win32_windowRefreshRate.h : 
//
// Win32_utilities

#pragma once

#include "win32_rootWindow.h"
#include "win32_refreshRate.h"

class WindowRefreshRate : public RefreshRate
{
public:
	WindowRefreshRate( RootWindow * const pRootWin )
		: m_pRootWin( pRootWin )
	{ }

	virtual void Trigger( )
	{
		m_pRootWin->Invalidate( FALSE );
	}

private:
	RootWindow * const m_pRootWin;
};
