// wrapperHelpers.cpp : wrapper functions for unit tests and scripting of application
//

#include "stdafx.h"
#include "assert.h"
#include "config.h"
#include "SCRIPT.H"
#include "EvolutionTypes.h"
#include "gridCircle.h"
#include "EvolutionCoreWrapperHelpers.h"

GridPoint ScrReadGridPoint( Script & script )
{
    short const x = script.ScrReadShort();
    short const y = script.ScrReadShort();
    return GridPoint( x, y );
}

GridCircle ScrReadGridCircle( Script & script )
{
    GridPoint  const gpCenter = ScrReadGridPoint( script );
    GRID_COORD const radius   = script.ScrReadUchar();
    return GridCircle( gpCenter, radius );
}

GridRect ScrReadGridRect( Script & script )
{
    GridPoint gpStart = ScrReadGridPoint( script );
    GridPoint gpEnd   = ScrReadGridPoint( script );
    return GridRect( gpStart, gpEnd );
}

