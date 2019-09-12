// win32_appMenu.h
//
// Evolution

#pragma once

#include "windef.h"

class EvoWorkThreadInterface;
class WinManager;

class AppMenu
{
public:
	AppMenu() :
		m_hMenu( nullptr ),
		m_pEvoWorkThreadInterface( nullptr ),
		m_pWinManager( nullptr )
	{}

	void Initialize( HWND const, EvoWorkThreadInterface const * const, WinManager const * const );
	void AdjustVisibility( );
	void Start( BOOL const );
	void Stop();

private:
	void enableMenues( UINT const );

	HMENU                          m_hMenu;
	EvoWorkThreadInterface const * m_pEvoWorkThreadInterface;
	WinManager             const * m_pWinManager;
};