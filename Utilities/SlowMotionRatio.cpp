// SlowMotionRatio.cpp 
//
// Utilities

#include "stdafx.h"
#include "SlowMotionRatio.h"

std::array< unsigned int, SlowMotionRatio::MAX_INDEX + 1 > const SlowMotionRatio::m_ratios
{
	10,      // 0
	20,      // 1
	50,      // 2
	100,     // 3
	200,     // 4
	500,     // 5
	1000,    // 6
	2000,    // 7
	5000,    // 8
	10000,   // 9
	20000,   // 10
	50000,   // 11
	100000,  // 12
	200000,  // 13
	500000,  // 14
	1000000  // 15
};
