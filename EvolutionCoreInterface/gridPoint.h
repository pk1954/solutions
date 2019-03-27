// gridPoint.h : 
//
// library EvolutionCoreInterface

#pragma once

#include <functional>
#include "gridCoord.h"
#include "PointType.h"

using GridPoint = PosType< GRID_COORD >;

inline bool IsEvenColumn( GridPoint const & gp ) { return IsEven( gp.GetX() ); }
inline bool IsOddColumn ( GridPoint const & gp ) { return IsOdd ( gp.GetX() ); }

using GridPointFunc   = std::function<void (GridPoint const)>;
using ManipulatorFunc = std::function<short(short const, short const)>;
