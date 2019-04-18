// win32_appMenu.h
//
// Evolution

#pragma once

#include "windef.h"

class WinManager;
class StatusBar;

class AppMenu
{
public:
	AppMenu( HWND const, WinManager * const, StatusBar * const );
	~AppMenu() {};

	void Start();
	void RunMode( BOOL const );
	void Stop();

private:
	void enableMenues( UINT );

	HMENU        m_hMenu;
	HWND         m_hwndApp;
	WinManager * m_pWinManager;
	StatusBar  * m_pStatusBar;
};