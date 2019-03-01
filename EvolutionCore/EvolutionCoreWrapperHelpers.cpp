// EvolutionCoreWrapperHelpers.cpp : wrapper functions for unit tests and scripting of application
//

#include "stdafx.h"
#include "assert.h"
#include "config.h"
#include "ERRHNDL.H"
#include "SCRIPT.H"
#include "EvolutionTypes.h"
#include "EvolutionCoreWrapperHelpers.h"

GRID_COORD ScrReadGridCoord( Script & script )
{
	unsigned short us = script.ScrReadUshort();
	if ( us > MAX_GRID_COORD.GetValue() )
		ScriptErrorHandler::throwError( 777, L"GRID_COORD too big" );
    return GRID_COORD( us );
}

GridPoint ScrReadGridPoint( Script & script )
{
    GRID_X const x(ScrReadGridCoord( script ));
    GRID_Y const y(ScrReadGridCoord( script ));
    return GridPoint( x, y );
}

GridRect ScrReadGridRect( Script & script )
{
    GridPoint gpStart(ScrReadGridPoint( script ));
    GridPoint gpEnd  (ScrReadGridPoint( script ));
    return GridRect( gpStart, gpEnd );
}
