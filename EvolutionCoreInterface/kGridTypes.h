// kGridTypes.h : 
//
// library EvolutionCoreInterface

#pragma once

#include "PixelTypes.h"
#include "PointType.h"
#include "RectType.h"

using KGridPoint    = PointType< long, struct KGrid_Parameter >;
using KGridRectSize = PointType< long, struct KGridRectSize_Parameter >;
using KGridRect     = RectType < long, KGridPoint, KGridRectSize, struct KGridRect_Parameter >;
