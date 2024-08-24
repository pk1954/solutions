// EvolutionCoreWrapperHelpers.ixx
//
// EvoCoreLib

export module EvoCoreLib:EvolutionCoreWrapperHelpers;

import RunTime;
import :GridCoord;
import :GridPoint;
import :GridRect;

extern GridCoord ScrReadGridCoord(Script &);
extern GridPoint ScrReadGridPoint(Script &);
extern GridRect  ScrReadGridRect (Script &);
