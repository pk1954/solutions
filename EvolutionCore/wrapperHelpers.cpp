// wrapperHelpers.cpp : wrapper functions for unit tests and scripting of application
//

#include "stdafx.h"
#include "assert.h"
#include "config.h"
#include "SCRIPT.H"
#include "EvolutionTypes.h"
#include "gridCircle.h"
#include "wrapperHelpers.h"

GridPoint ScrReadGridPoint( Script & script )
{
    short const x = script.ScrReadShort();
    short const y = script.ScrReadShort();
    return GridPoint( x, y );
}

GridCircle ScrReadGridCircle( Script & script )
{
    GridPoint const gpCenter = ScrReadGridPoint( script );
    short     const sRadius  = script.ScrReadShort();
    return GridCircle( gpCenter, sRadius );
}

GridRect ScrReadGridRect( Script & script )
{
    GridPoint gpStart = ScrReadGridPoint( script );
    GridPoint gpEnd   = ScrReadGridPoint( script );
    return GridRect( gpStart, gpEnd );
}

