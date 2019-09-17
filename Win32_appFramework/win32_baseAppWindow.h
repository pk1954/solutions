// win32_baseAppWindow.h
//
// Win32_appFramework

#pragma once

#include "win32_baseWindow.h"
#include "win32_winManager.h"

class WorkThreadInterface;
class HistorySystem;
class ModelWindow;
class HistWindow;
class StatusBar;

class BaseAppWindow : public BaseWindow
{
public:
	BaseAppWindow( );
	void Start
	( 
		HWND const,
		WorkThreadInterface * const
	);
	void Stop( );
	virtual ~BaseAppWindow(); 

	void AdjustChildWindows( ModelWindow * const );



protected:
	HWND         m_hwndConsole;
	WinManager   m_WinManager;

	StatusBar     * m_pStatusBar;
	HistWindow    * m_pHistWindow;
	HistorySystem * m_pHistorySystem;
};