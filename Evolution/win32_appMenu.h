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
	void Start( BOOL const );
	void AdjustVisibility( );
	void Stop();

private:
	void enableMenues( UINT const );
	void disableIfVisible( UINT const id )
	{
		EnableMenuItem( m_hMenu, id, m_pWinManager->IsVisible( id ) );
	}

	HMENU                       m_hMenu;
	WorkThreadInterface const * m_pWorkThreadInterface;
	WinManager          const * m_pWinManager;
};