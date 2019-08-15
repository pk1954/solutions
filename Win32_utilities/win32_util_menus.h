// win32_util_menus.h
//
// win32_utilities

#pragma once

#include "windef.h"

namespace Util
{
	//void AddHistWinMenu( HMENU const );
	//void AdjustHistWinMenu( HMENU const, tOnOffAuto const );
	void AddPerfWinMenu( HMENU const );
	void AdjustPerfWinMenu( HMENU const, tOnOffAuto const );
}
