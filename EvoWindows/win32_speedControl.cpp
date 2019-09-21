// win32_speedControl.cpp
//
// EvoWindows

#include "stdafx.h"
#include "LogarithmicTrackbar.h"
#include "win32_speedControl.h"

StatusBar     * SpeedControl::m_pStatusBar           = nullptr;
HistorySystem * SpeedControl::m_pHistorySystem       = nullptr;

long const SpeedControl::MAX_DELAY = 2048;    // in msecs

long const SpeedControl::SPEED_TRACKBAR_MIN = 0; 
long const SpeedControl::SPEED_TRACKBAR_MAX = LogarithmicTrackbar::Value2Trackbar( MAX_DELAY ); 
