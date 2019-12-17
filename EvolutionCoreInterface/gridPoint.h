// gridPoint.h : 
//
// EvolutionCoreInterface

#pragma once

#include <functional>
#include "gridCoord.h"
#include "PointType.h"

using GridPoint = PosType< GRID_COORD >;

static GridPoint const GP_NULL( GridPoint::NULL_VAL() );   // compiler generates call!
static GridPoint const GP_ZERO( GridPoint::ZERO_VAL() );   // compiler generates call!

inline bool IsEvenColumn( GridPoint const & gp ) { return IsEven( gp.GetX() ); }
inline bool IsOddColumn ( GridPoint const & gp ) { return IsOdd ( gp.GetX() ); }

using GridPointFunc     = function<void (GridPoint const)>;
using GridPointBoolFunc = function<bool (GridPoint const)>;
using ManipulatorFunc   = function<short(short const, short const)>;
