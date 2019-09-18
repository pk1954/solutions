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
	WinManager m_WinManager;

	AppMenu       * m_pAppMenu;
	StatusBar     * m_pStatusBar;
	HistorySystem * m_pHistorySystem;  //TODO: make private

private:

	HWND m_hwndConsole;
	BOOL m_bStarted;  // if true, model is visible, all functions available

	Controller          * m_pController;
	HistWindow          * m_pHistWindow;
	ModelWindow         * m_pModelWindow;
	HistInfoWindow      * m_pHistInfoWindow;
	WorkThreadInterface * m_pWorkThreadInterface;

	virtual LRESULT UserProc( UINT const, WPARAM const, LPARAM const );
};