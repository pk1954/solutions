// SlowMotionRatio.cpp 
//
// Utilities

#include "stdafx.h"
#include "SlowMotionRatio.h"

std::array< float, SlowMotionRatio::MAX_INDEX + 1 > const SlowMotionRatio::m_ratios
{
	1.0f,       // 0
	2.0f,       // 1
	5.0f,       // 2
	10.0f,      // 3
	20.0f,      // 4
	50.0f,      // 5
	100.0f,     // 6
	200.0f,     // 7
	500.0f,     // 8
	1000.0f,    // 9
	2000.0f,    // 10
	5000.0f,    // 11
	10000.0f,   // 12
	20000.0f,   // 13
	50000.0f,   // 14
	100000.0f,  // 15
	200000.0f,  // 16
	500000.0f,  // 17
	1000000.0f  // 18
};
