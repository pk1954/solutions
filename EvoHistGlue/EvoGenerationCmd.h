// EvoGenerationCmd.h
//

#pragma once

#include "GenerationCmd.h"

enum class tEvoCmd : int8_t // enumeration starts after last tGenCmd value
{
	nextGen = static_cast<int8_t>(tGenCmd::FIRST_APP_CMD),
    reset,
	setSimulationMode,   // simulation mode / edit mode
    editSetBrushMode,
    editSetBrushShape,
    editSetBrushManipulator,
    editSetBrushRadius,
    editSetBrushIntensity,
	editSetPOI,
	editDoEdit,
	LAST_APP_CMD = editDoEdit
};

bool IsEditorCommand( tEvoCmd const );

wchar_t const * const GetEvoCommandName     ( tEvoCmd const );
wchar_t const * const GetEvoCommandNameShort( tEvoCmd const );

std::wostream & operator << ( std::wostream &, GenerationCmd const & );
