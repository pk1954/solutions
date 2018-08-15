// EvoGenerationCmd.h
//

#pragma once

#include "GenerationCmd.h"

enum class tEvoCmd : int16_t // enumeration starts after last tGenCmd value
{
	nextGen = GenerationCmd::FIRST_APP_CMD,
    reset,
	setSimulationMode,   // simulation mode / edit mode
    editSetBrushMode,
    editSetBrushShape,
    editSetBrushSize,
    editSetBrushIntensity,
    editDoEdit,
	editSetPOI
};

bool IsEditorCommand( tEvoCmd const );

wchar_t const * const GetEvoCommandName     ( tEvoCmd const );
wchar_t const * const GetEvoCommandNameShort( tEvoCmd const );

wostream & operator << ( wostream &, GenerationCmd const & );
