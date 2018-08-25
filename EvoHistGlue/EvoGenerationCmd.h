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
    editSetBrushOperator,
    editSetBrushSize,
    editSetBrushIntensity,
    editDoEdit,
	editSetPOI,
	LAST_APP_CMD = editSetPOI
};

bool IsEditorCommand( tEvoCmd const );

wchar_t const * const GetEvoCommandName     ( tEvoCmd const );
wchar_t const * const GetEvoCommandNameShort( tEvoCmd const );

wostream & operator << ( wostream &, GenerationCmd const & );
