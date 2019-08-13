// win32_appMenu.h
//
// Evolution

#pragma once

#include "windef.h"

class WorkThreadInterface;

class AppMenu
{
public:
	AppMenu() :
		m_hMenu( nullptr ),
		m_pWorkThreadInterface( nullptr )
	{}

	void Initialize( HWND const, WorkThreadInterface const * const );
	void Start( BOOL const );
	void AdjustVisibility( );
	void Stop();

private:
	void enableMenues( UINT );

	HMENU                       m_hMenu;
	WorkThreadInterface const * m_pWorkThreadInterface;
};