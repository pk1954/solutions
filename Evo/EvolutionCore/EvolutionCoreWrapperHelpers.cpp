// EvolutionCoreWrapperHelpers.cpp : wrapper functions for unit tests and scripting of application
//

#include <cassert>
#include "ERRHNDL.H"
#include "SCRIPT.H"
#include "EvolutionTypes.h"
#include "EvolutionCoreWrapperHelpers.h"

import Config;

GRID_COORD ScrReadGridCoord(Script & script)
{
	unsigned short us = script.ScrReadUshort();
	if (us > MAX_GRID_COORD.GetValue())
		ScriptErrorHandler::throwError(777, L"GRID_COORD too big");
    return GRID_COORD(us);
}

GridPoint ScrReadGridPoint(Script & script)
{
	script.ScrReadSpecial('(');
    GRID_COORD const x(ScrReadGridCoord(script));
	script.ScrReadSpecial('/');
	GRID_COORD const y(ScrReadGridCoord(script));
	script.ScrReadSpecial(')');
	return GridPoint(x, y);
}

GridRect ScrReadGridRect(Script & script)
{
    GridPoint gpStart(ScrReadGridPoint(script));
    GridPoint gpEnd  (ScrReadGridPoint(script));
    return GridRect(gpStart, gpEnd);
}
