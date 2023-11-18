// GridPoint.ixx : 
//
// EvolutionCoreInterface

module;

#include <functional>

export module GridPoint;

import Types;
import GridCoord;

export using GridPoint = PosType< GridCoord >;

export static GridPoint const GP_NULL(GridPoint::NULL_VAL());   // compiler generates call!
export static GridPoint const GP_ZERO(GridPoint::ZERO_VAL());   // compiler generates call!

export inline bool IsEvenColumn(GridPoint const& gp) { return IsEven(gp.GetX()); }
export inline bool IsOddColumn(GridPoint const& gp) { return IsOdd(gp.GetX()); }

export using GridPointFunc = function<void(GridPoint const)>;
export using GridPointBoolFunc = function<bool(GridPoint const)>;
export using ManipulatorFunc = function<short(short const, short const)>;
