// win32_appMenu.h
//
// Evolution

#pragma once

#include "windef.h"

class WorkThreadInterface;
class WinManager;

class AppMenu
{
public:
	virtual ~AppMenu() {}

	virtual void Initialize( HWND const, WorkThreadInterface const * const, WinManager const * const ) = 0;
	virtual void AdjustVisibility( ) = 0;
	virtual void Start( ) = 0;
	virtual void Stop( ) = 0;
};
