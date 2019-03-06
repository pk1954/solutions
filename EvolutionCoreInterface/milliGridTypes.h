// milliGridTypes.h : 
//
// library EvolutionCoreInterface

#pragma once

#include "PixelTypes.h"
#include "PointType.h"
#include "RectType.h"

using MilliGrid         = NamedType< long, struct MilliGrid_Parameter >;
using MilliGridPoint    = PosType  < MilliGrid >;
using MilliGridRectSize = SizeType < MilliGrid >;
using MilliGridRect     = RectType < MilliGrid >;

using MilliGrid_X = NamedType<MilliGrid, x_tag>;
using MilliGrid_Y = NamedType<MilliGrid, y_tag>;
