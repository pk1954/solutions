// win32_appMenu.h
//
// Evolution

#pragma once

#include "windef.h"

class AppMenu
{
public:
	void Initialize( HWND const );
	void Start();
	void RunMode( BOOL const );
	void Stop();

private:
	void enableMenues( UINT );

	HMENU m_hMenu;
};