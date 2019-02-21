// gridPoint.h : 
//
// library EvolutionCoreInterface

#pragma once

#include <functional>
#include "gridCoord.h"
#include "PointType.h"

using GRID_X = NamedType<GRID_COORD, x_tag>;
using GRID_Y = NamedType<GRID_COORD, y_tag>;

using GridPoint = PosType< GRID_COORD >;

inline bool IsEvenColumn( GridPoint const & gp ) { return IsEven( gp.GetX().GetValue() ); }
inline bool IsOddColumn ( GridPoint const & gp ) { return IsOdd ( gp.GetX().GetValue() ); }

using GridPointFunc   = std::function<void (GridPoint const)>;
using ManipulatorFunc = std::function<short(short const, short const)>;
