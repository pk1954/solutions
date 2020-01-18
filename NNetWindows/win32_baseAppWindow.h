// win32_baseAppWindow.h
//
// NNetWindows

#pragma once

#include <fstream>
#include <iostream>
#include "win32_baseWindow.h"
#include "win32_winManager.h"
#include "win32_status.h"

class NNetWorkThreadInterface;
class ModelWindow;
class AppMenu;

class BaseAppWindow : public BaseWindow
{
public:
	BaseAppWindow( );
	virtual ~BaseAppWindow(); 

	void Initialize( NNetWorkThreadInterface * const );

	void Start( ModelWindow * const );

	virtual void ProcessAppCommand( WPARAM const, LPARAM const = 0 ) = 0;

protected:
	HWND       m_hwndApp;
	WinManager m_WinManager;
	StatusBar  m_StatusBar;
	AppMenu  * m_pAppMenu;        // allocated by application

	virtual void Stop( );

    bool ProcessFrameworkCommand( WPARAM const, LPARAM const = 0 );

private:

	HWND m_hwndConsole;
	BOOL m_bStarted;          // if true, model is visible, all functions available

	ModelWindow             * m_pModelWindow;
	NNetWorkThreadInterface * m_pWorkThreadInterface;

	std::wofstream m_traceStream;

	void adjustChildWindows( );

	virtual LRESULT UserProc( UINT const, WPARAM const, LPARAM const );
};