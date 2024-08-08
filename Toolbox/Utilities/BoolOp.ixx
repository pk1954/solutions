// BoolOp.ixx : 
//
// Toolbox\Utilities

export module BoolOp;

import std;
using std::wostream;

export enum class tBoolOp { opTrue, opFalse, opToggle, opNoChange };

export inline tBoolOp BoolOp(bool const b)
{
    return b ? tBoolOp::opTrue : tBoolOp::opFalse;
}

export inline tBoolOp Reverse(tBoolOp const op)
{
    switch (op)
    {
    case tBoolOp::opTrue:
        return tBoolOp::opFalse;

    case tBoolOp::opFalse:
        return tBoolOp::opTrue;

    case tBoolOp::opToggle:
        return tBoolOp::opToggle;

    case tBoolOp::opNoChange: 
        return tBoolOp::opNoChange;

    default:
        ;   //assert(false);
        return tBoolOp::opNoChange;
    }
}

export inline bool ApplyOp2(bool const b, tBoolOp const op)
{
    switch (op)
    {
    case tBoolOp::opTrue:
        return true;

    case tBoolOp::opFalse:
        return false;

    case tBoolOp::opToggle:
        return ! b;

    case tBoolOp::opNoChange: 
        return b;

    default:
        ;  //assert(false);
        return false;
    }
}

export inline void ApplyOp(bool & b, tBoolOp const op)
{
    b = ApplyOp2(b, op);
}

export wchar_t const * GetBoolOpName(tBoolOp const op)
{
    static std::unordered_map < tBoolOp, wchar_t const * const > mapNames =
    {
        { tBoolOp::opTrue,     L"tBoolOp::opTrue"     },  
        { tBoolOp::opFalse,    L"tBoolOp::opFalse"    },  
        { tBoolOp::opToggle,   L"tBoolOp::opToggle"   },  
        { tBoolOp::opNoChange, L"tBoolOp::opNoChange" }
    };

    return mapNames.at(op);
}

export wostream & operator<< (wostream & out, tBoolOp const op)
{
    out << L" " << std::wstring(GetBoolOpName(op));
    return out;
}
