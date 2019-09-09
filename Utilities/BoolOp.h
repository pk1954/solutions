// BoolOp.h : 
//

#include "assert.h"

#pragma once

enum class tBoolOp { opTrue, opFalse, opToggle, opNoChange };

inline tBoolOp BoolOp( bool const b )
{
	return b ? tBoolOp::opTrue : tBoolOp::opFalse;
}

inline bool ApplyOp2( bool const b, tBoolOp const op )
{
    switch ( op )
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
        assert( false );
        return false;
    }
}

inline void ApplyOp( bool & b, tBoolOp const op )
{
    b = ApplyOp2( b, op );
}

wchar_t const * const GetBoolOpName( tBoolOp const );
