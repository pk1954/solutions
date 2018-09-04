// wrapperHelpers.cpp : wrapper functions for unit tests and scripting of application
//

#include "stdafx.h"
#include "assert.h"
#include "config.h"
#include "SCRIPT.H"
#include "EvolutionTypes.h"
#include "pixelCoordinates.h"
#include "EvolutionCoreWrapperHelpers.h"

GridPoint ScrReadGridPoint( Script & script )
{
    short const x = script.ScrReadShort();
    short const y = script.ScrReadShort();
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
