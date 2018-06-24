// UtilityWrappers.cpp
//

#include "stdafx.h"
#include "assert.h"
#include "SCRIPT.H"
#include "symtab.h"
#include "trace.h"
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
}

