// win32_baseAppWindow.h
//
// Win32_appFramework

#pragma once

#include "win32_baseWindow.h"
#include "win32_winManager.h"

class WorkThreadInterface;
class HistInfoWindow;
class HistorySystem;
class ModelWindow;
class HistWindow;
class StatusBar;

class BaseAppWindow : public BaseWindow
{
public:
	BaseAppWindow( WorkThreadInterface * const );
	void Start( ModelWindow * const, HWND const );
	void Stop( );
	virtual ~BaseAppWindow(); 

	void AdjustChildWindows( );

protected:
	HWND       m_hwndConsole;
	WinManager m_WinManager;

	StatusBar           * m_pStatusBar;
	HistWindow          * m_pHistWindow;
	ModelWindow         * m_pModelWindow;
	HistorySystem       * m_pHistorySystem;
	HistInfoWindow      * m_pHistInfoWindow;
	WorkThreadInterface * m_pWorkThreadInterface;
};