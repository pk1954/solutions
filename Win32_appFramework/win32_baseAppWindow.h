// win32_baseAppWindow.h
//
// Win32_appFramework

#pragma once

#include "win32_baseWindow.h"
#include "win32_winManager.h"
#include "win32_histWindow.h"
#include "win32_historyInfo.h"
#include "win32_status.h"

class WorkThreadInterface;
class HistorySystem;
class ModelWindow;
class Controller;
class AppMenu;

class BaseAppWindow : public BaseWindow
{
public:
	BaseAppWindow( WorkThreadInterface * const );
	void Start( ModelWindow * const, HWND const, Controller * const );
	void Stop( );
	virtual ~BaseAppWindow(); 

	void AdjustChildWindows( );

protected:
	HWND       m_hwndApp;
	WinManager m_WinManager;
	StatusBar  m_StatusBar;

	AppMenu       * m_pAppMenu;        // allocated by application
	HistorySystem * m_pHistorySystem;  // allocated here

	std::wofstream m_traceStream;

private:

	HWND m_hwndConsole;
	BOOL m_bStarted;  // if true, model is visible, all functions available

	HistWindow     m_HistWindow;
	HistInfoWindow m_HistInfoWindow;

	Controller          * m_pController;
	ModelWindow         * m_pModelWindow;
	WorkThreadInterface * m_pWorkThreadInterface;

	virtual LRESULT UserProc( UINT const, WPARAM const, LPARAM const );
};