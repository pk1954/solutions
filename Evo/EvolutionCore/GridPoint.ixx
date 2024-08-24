// GridPoint.ixx : 
//
// EvoCoreLib

export module EvoCoreLib:GridPoint;

import std;
import Types;
import IoConstants;
import :GridCoord;

using std::function;
using std::wostream;

export using GridPoint = PosType<GridCoord>;

export GridPoint const GP_NULL(GridPoint::NULL_VAL());   // compiler generates call!
export GridPoint const GP_ZERO(GridPoint::ZERO_VAL());   // compiler generates call!

export inline bool IsEvenColumn(GridPoint const& gp) { return IsEven(gp.GetX()); }
export inline bool IsOddColumn (GridPoint const& gp) { return IsOdd(gp.GetX()); }

export using GridPointFunc     = function<void(GridPoint const)>;
export using GridPointBoolFunc = function<bool(GridPoint const)>;
export using ManipulatorFunc   = function<short(short const, short const)>;

export wostream& operator<< (wostream& out, GridPoint const& gp)
{
    out << gp.GetX() << SPACE << gp.GetY();  //TODO:format correct?
    return out;
}
