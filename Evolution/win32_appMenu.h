// win32_appMenu.h
//
// Evolution

#pragma once

#include "windef.h"

class AppMenu
{
public:
	AppMenu( HWND const );
	~AppMenu() {};

	void Start();
	void RunMode( BOOL const );
	void Stop();

private:
	void enableMenues( UINT );

	HMENU m_hMenu;
	HWND  m_hwndApp;
};