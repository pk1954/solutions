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
class AppMenu;

class BaseAppWindow : public BaseWindow
{
public:
	BaseAppWindow( );
	virtual ~BaseAppWindow(); 

	void Initialize( WorkThreadInterface * const );

	virtual void ProcessAppCommand( WPARAM const, LPARAM const = 0 ) = 0;

protected:
	HWND       m_hwndApp;
	WinManager m_WinManager;
	StatusBar  m_StatusBar;

	AppMenu       * m_pAppMenu;        // allocated by application
	HistorySystem * m_pHistorySystem;  // allocated here

	void Start( ModelWindow * const );
	void Stop( );

    bool ProcessFrameworkCommand( WPARAM const, LPARAM const = 0 );

private:

	HWND m_hwndConsole;
	BOOL m_bStarted;  // if true, model is visible, all functions available

	HistWindow     m_HistWindow;
	HistInfoWindow m_HistInfoWindow;

	ModelWindow         * m_pModelWindow;
	WorkThreadInterface * m_pWorkThreadInterface;

	std::wofstream m_traceStream;

	void adjustChildWindows( );

	virtual LRESULT UserProc( UINT const, WPARAM const, LPARAM const );
};