// win32_menus.cpp
//
// EvoWindows

#include "stdafx.h"
#include "resource.h"
#include "config.h"
#include "win32_menus.h"

void AddMiniWinMenu( HMENU const hPopupMenu )
{
	UINT  const STD_FLAGS    = MF_BYPOSITION | MF_STRING;
	HMENU const hMiniWinMenu = CreatePopupMenu();
	(void)AppendMenu( hMiniWinMenu, STD_FLAGS, IDM_MINI_WINDOW_AUTO, L"auto" );
	(void)AppendMenu( hMiniWinMenu, STD_FLAGS, IDM_MINI_WINDOW_ON,   L"on" );
	(void)AppendMenu( hMiniWinMenu, STD_FLAGS, IDM_MINI_WINDOW_OFF,  L"off" );
	(void)AppendMenu( hPopupMenu, MF_BYPOSITION | MF_POPUP, (UINT_PTR)hMiniWinMenu, L"Mini window" );
}

void AdjustMiniWinMenu( HMENU const hMenu )
{
	tOnOffAuto const onOffAuto = Config::GetConfigValueOnOffAuto( Config::tId::miniGridDisplay );
	EnableMenuItem( hMenu, IDM_MINI_WINDOW_AUTO, ((onOffAuto == tOnOffAuto::automatic ) ? MF_GRAYED : MF_ENABLED) );
	EnableMenuItem( hMenu, IDM_MINI_WINDOW_ON,   ((onOffAuto == tOnOffAuto::on        ) ? MF_GRAYED : MF_ENABLED) );
	EnableMenuItem( hMenu, IDM_MINI_WINDOW_OFF,  ((onOffAuto == tOnOffAuto::off       ) ? MF_GRAYED : MF_ENABLED) );
}	
