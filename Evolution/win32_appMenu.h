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
	AppMenu() :
		m_hMenu( nullptr ),
		m_pWorkThreadInterface( nullptr ),
		m_pWinManager( nullptr )
	{}

	void Initialize( HWND const, WorkThreadInterface const * const, WinManager const * const );
	void AdjustVisibility( );
	void Start( BOOL const );
	void Stop();

private:
	void enableMenues( UINT const );

	HMENU                       m_hMenu;
	WorkThreadInterface const * m_pWorkThreadInterface;
	WinManager          const * m_pWinManager;
};