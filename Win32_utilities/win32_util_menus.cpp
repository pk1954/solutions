// win32_menus.cpp
//
// win32_utilities

#include "stdafx.h"
#include "util.h"
#include "win32_util_resource.h"
#include "win32_util_menus.h"

namespace Util
{
	//void AddHistWinMenu( HMENU const hMenuParent )
	//{
	//	UINT  const STD_FLAGS = MF_BYPOSITION | MF_STRING;
	//	HMENU const hMenu = CreatePopupMenu();
	//	(void)AppendMenu( hMenu, STD_FLAGS, IDM_HIST_WINDOW_AUTO, L"auto" );
	//	(void)AppendMenu( hMenu, STD_FLAGS, IDM_HIST_WINDOW_ON,  L"on" );
	//	(void)AppendMenu( hMenu, STD_FLAGS, IDM_HIST_WINDOW_OFF, L"off" );
	//	(void)AppendMenu( hMenuParent, MF_BYPOSITION | MF_POPUP, (UINT_PTR)hMenu, L"Hist window" );
	//}

	//void AdjustHistWinMenu( HMENU const hMenu, tOnOffAuto const onOffAuto )
	//{
	//	EnableMenuItem( hMenu, IDM_HIST_WINDOW_AUTO, ((onOffAuto == tOnOffAuto::automatic ) ? MF_GRAYED : MF_ENABLED) );
	//	EnableMenuItem( hMenu, IDM_HIST_WINDOW_ON,   ((onOffAuto == tOnOffAuto::on        ) ? MF_GRAYED : MF_ENABLED) );
	//	EnableMenuItem( hMenu, IDM_HIST_WINDOW_OFF,  ((onOffAuto == tOnOffAuto::off       ) ? MF_GRAYED : MF_ENABLED) );
	//}

	void AddPerfWinMenu( HMENU const hMenuParent )
	{
		UINT  const STD_FLAGS = MF_BYPOSITION | MF_STRING;
		HMENU const hMenu = CreatePopupMenu();
		(void)AppendMenu( hMenu, STD_FLAGS, IDM_PERF_WINDOW_AUTO, L"auto" );
		(void)AppendMenu( hMenu, STD_FLAGS, IDM_PERF_WINDOW_ON,  L"on" );
		(void)AppendMenu( hMenu, STD_FLAGS, IDM_PERF_WINDOW_OFF, L"off" );
		(void)AppendMenu( hMenuParent, MF_BYPOSITION | MF_POPUP, (UINT_PTR)hMenu, L"Perf window" );
	}

	void AdjustPerfWinMenu( HMENU const hMenu, tOnOffAuto const onOffAuto )
	{
		EnableMenuItem( hMenu, IDM_PERF_WINDOW_AUTO, ((onOffAuto == tOnOffAuto::automatic ) ? MF_GRAYED : MF_ENABLED) );
		EnableMenuItem( hMenu, IDM_PERF_WINDOW_ON,   ((onOffAuto == tOnOffAuto::on        ) ? MF_GRAYED : MF_ENABLED) );
		EnableMenuItem( hMenu, IDM_PERF_WINDOW_OFF,  ((onOffAuto == tOnOffAuto::off       ) ? MF_GRAYED : MF_ENABLED) );
	}
}