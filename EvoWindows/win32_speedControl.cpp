// win32_speedControl.cpp
//
// EvoWindows

#include "stdafx.h"
#include "LogarithmicTrackbar.h"
#include "win32_speedControl.h"

BOOL SpeedControl::m_bWithHistory = FALSE;

StatusBar           * SpeedControl::m_pStatusBar           = nullptr;
WorkThreadInterface * SpeedControl::m_pWorkThreadInterface = nullptr;
HistorySystem       * SpeedControl::m_pHistorySystem       = nullptr;

long const SpeedControl::MAX_DELAY = 2048;    // in msecs

long const SpeedControl::SPEED_TRACKBAR_MIN = 0; 
long const SpeedControl::SPEED_TRACKBAR_MAX = LogarithmicTrackbar::Value2Trackbar( MAX_DELAY ); 
