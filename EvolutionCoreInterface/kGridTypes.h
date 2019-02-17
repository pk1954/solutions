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
