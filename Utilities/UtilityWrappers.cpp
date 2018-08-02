// UtilityWrappers.cpp
//

#include "stdafx.h"
#include "assert.h"
#include "SCRIPT.H"
#include "symtab.h"
#include "trace.h"
#include "BoolOp.h"
#include "UtilityWrappers.h"

class WrapOpenTraceFile : public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
		OpenTraceFile( script.ScrReadString( ) );
    }
};

void DefineUtilityWrapperFunctions( )
{
    DEF_FUNC( OpenTraceFile );

    DEF_ULONG_CONST( tBoolOp::opTrue );
    DEF_ULONG_CONST( tBoolOp::opFalse );
    DEF_ULONG_CONST( tBoolOp::opToggle );
    DEF_ULONG_CONST( tBoolOp::opNoChange );
}
