// EvolutionCoreWrapperHelpers.cpp : wrapper functions for unit tests and scripting of application
//

#include "ERRHNDL.H"
#include "SCRIPT.H"
import EvolutionTypes;
#include "EvolutionCoreWrapperHelpers.h"

import Debug;
import Config;

GridCoord ScrReadGridCoord(Script & script)
{
	unsigned short us = script.ScrReadUshort();
	if (us > MAX_GRID_COORD.GetValue())
		ScriptErrorHandler::throwError(777, L"GridCoord too big");
    return GridCoord(us);
}

GridPoint ScrReadGridPoint(Script & script)
{
	script.ScrReadSpecial('(');
    GridCoord const x(ScrReadGridCoord(script));
	script.ScrReadSpecial('/');
	GridCoord const y(ScrReadGridCoord(script));
	script.ScrReadSpecial(')');
	return GridPoint(x, y);
}

GridRect ScrReadGridRect(Script & script)
{
    GridPoint gpStart(ScrReadGridPoint(script));
    GridPoint gpEnd  (ScrReadGridPoint(script));
    return GridRect(gpStart, gpEnd);
}
