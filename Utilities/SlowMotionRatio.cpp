// SlowMotionRatio.cpp 
//
// Utilities

#include "stdafx.h"
#include "SlowMotionRatio.h"

std::array< unsigned int, SlowMotionRatio::MAX_INDEX + 1 > const SlowMotionRatio::m_ratios
{
	1,       // 0
	2,       // 1
	5,       // 2
	10,      // 3
	20,      // 4
	50,      // 5
	100,     // 6
	200,     // 7
	500,     // 8
	1000,    // 9
	2000,    // 10
	5000,    // 11
	10000,   // 12
	20000,   // 13
	50000,   // 14
	100000,  // 15
	200000,  // 16
	500000,  // 17
	1000000  // 18
};
