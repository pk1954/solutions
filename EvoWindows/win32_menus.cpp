// win32_menus.cpp
//
// EvoWindows

#include "stdafx.h"
#include "resource.h"
#include "config.h"
#include "win32_util_menus.h"
#include "win32_menus.h"

void AddMiniWinMenu( HMENU const hMenuParent )
{
	UINT  const STD_FLAGS    = MF_BYPOSITION | MF_STRING;
	HMENU const hMenu = CreatePopupMenu();
	(void)AppendMenu( hMenu, STD_FLAGS, IDM_MINI_WINDOW_AUTO, L"auto" );
	(void)AppendMenu( hMenu, STD_FLAGS, IDM_MINI_WINDOW_ON,   L"on" );
	(void)AppendMenu( hMenu, STD_FLAGS, IDM_MINI_WINDOW_OFF,  L"off" );
	(void)AppendMenu( hMenuParent, MF_BYPOSITION | MF_POPUP, (UINT_PTR)hMenu, L"Mini window" );
}

void AdjustMiniWinMenu( HMENU const hMenu )
{
	tOnOffAuto const onOffAuto = Config::GetConfigValueOnOffAuto( Config::tId::miniGridDisplay );
	EnableMenuItem( hMenu, IDM_MINI_WINDOW_AUTO, ((onOffAuto == tOnOffAuto::automatic ) ? MF_GRAYED : MF_ENABLED) );
	EnableMenuItem( hMenu, IDM_MINI_WINDOW_ON,   ((onOffAuto == tOnOffAuto::on        ) ? MF_GRAYED : MF_ENABLED) );
	EnableMenuItem( hMenu, IDM_MINI_WINDOW_OFF,  ((onOffAuto == tOnOffAuto::off       ) ? MF_GRAYED : MF_ENABLED) );
}

void AdjustHistWinMenu( HMENU const hMenu )
{
	tOnOffAuto const onOffAuto = Config::GetConfigValueOnOffAuto( Config::tId::historyDisplay );
	Util::AdjustHistWinMenu( hMenu, onOffAuto );
}

void AdjustPerfWinMenu( HMENU const hMenu )
{
	tOnOffAuto const onOffAuto = Config::GetConfigValueOnOffAuto( Config::tId::performanceDisplay );
	Util::AdjustPerfWinMenu( hMenu, onOffAuto );
}
