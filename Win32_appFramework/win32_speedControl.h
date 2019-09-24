// win32_speedControl.h
//
// win32_appFramework

#pragma once

#include "LogarithmicTrackbar.h"
#include "win32_util_resource.h"
#include "win32_WorkThreadInterface.h"
#include "win32_status.h"

class SpeedControl
{
public:

	static void SetSpeedTrackBar( long const );
	static void AddSimulationControl( StatusBar * const, HistorySystem * const, long const, long const, long const );
	static void Adjust( BOOL const,	WorkThreadInterface * const );

private:
	static StatusBar     * m_pStatusBar;
	static HistorySystem * m_pHistorySystem;
};
