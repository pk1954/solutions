// win32_appMenu.h
//
// NNetWindows

#pragma once

#include "windef.h"

class NNetWorkThreadInterface;
class WinManager;

class AppMenu
{
public:
	virtual ~AppMenu() {}

	virtual void Initialize( HWND const, NNetWorkThreadInterface const * const, WinManager const * const ) = 0;
	virtual void AdjustVisibility( ) = 0;
	virtual void Start( ) = 0;
	virtual void Stop( ) = 0;
};
