// EvoGenerationCmd.h
//

#pragma once

#include "GenerationCmd.h"

enum class tEvoCmd : unsigned short // enumeration starts after last tGenCmd value
{
    reset = GenerationCmd::FIRST_APP_CMD,
	nextGen,
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
