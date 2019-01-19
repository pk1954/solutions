// wrapperHelpers.cpp : wrapper functions for unit tests and scripting of application
//

#include "stdafx.h"
#include "assert.h"
#include "config.h"
#include "SCRIPT.H"
#include "EvolutionTypes.h"
#include "pixelCoordinates.h"
#include "EvolutionCoreWrapperHelpers.h"

GRID_COORD ScrReadGridCoord( Script & script )
{
    return GRID_COORD( script.ScrReadUshort() );
}

GridPoint ScrReadGridPoint( Script & script )
{
    GRID_COORD const x = ScrReadGridCoord( script );
    GRID_COORD const y = ScrReadGridCoord( script );
    return GridPoint( x, y );
}

GridRect ScrReadGridRect( Script & script )
{
    GridPoint gpStart = ScrReadGridPoint( script );
    GridPoint gpEnd   = ScrReadGridPoint( script );
    return GridRect( gpStart, gpEnd );
}

PixelPoint ScrReadPixelPoint( Script & script )
{
    long const x = script.ScrReadLong();
    long const y = script.ScrReadLong();
    return PixelPoint( x, y );
}
