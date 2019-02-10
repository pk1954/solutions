// kGridRect.h : 
//
// library Utilities

#pragma once

#include "kGridPoint.h"
#include "RectType.h"

using KGridRect = RectType < long, KGridPoint, KGridRectSize, struct KGridRect_Parameter >;

inline static KGridRect const & KGRID_RECT_EMPTY() 
{ 
	static KGridRect res( KGridPoint::ZERO_VAL(), KGridPoint::ZERO_VAL() ); 
	return res;
};
