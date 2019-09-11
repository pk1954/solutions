// win32_NNetSimuMenu.h
//
// NNetSimuMenu

#pragma once

class WinManager;

class NNetSimuMenu
{
public:
	NNetSimuMenu() :
		m_hMenu( nullptr ),
		m_pWinManager( nullptr )
	{}

	void Initialize( HWND const, WinManager const * const );
	void AdjustVisibility( );
	void Start( );
	void Stop( );

private:
	void enableMenues( UINT const );

	HMENU              m_hMenu;
	WinManager const * m_pWinManager;
};