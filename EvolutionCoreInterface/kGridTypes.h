// kGridTypes.h : 
//
// library EvolutionCoreInterface

#pragma once

#include "PixelTypes.h"
#include "PointType.h"
#include "RectType.h"

using KGrid         = NamedType< long, struct KGrid_Parameter >;
using KGridPoint    = PosType  < KGrid >;
using KGridRectSize = SizeType < KGrid >;
using KGridRect     = RectType < KGrid >;

using KGrid_X = NamedType<KGrid, x_tag>;
using KGrid_Y = NamedType<KGrid, y_tag>;
