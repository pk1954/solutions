// NNetGenerationCmd.h
//
// NNetHistGlue

#pragma once

#include "GenerationCmd.h"

enum class tNNetCmd : int8_t // enumeration starts after last tGenCmd value
{
	nextGen = static_cast<int8_t>(tGenCmd::FIRST_APP_CMD),
    reset,
    editFirst,  // placeholder
	editLast,   // placeholder
	FIRST_EDIT_CMD = editFirst,
	LAST_APP_CMD   = editLast
};

bool IsEditorCommand( tNNetCmd const );

wchar_t const * const GetNNetCommandName     ( tNNetCmd const );
wchar_t const * const GetNNetCommandNameShort( tNNetCmd const );

std::wostream & operator << ( std::wostream &, GenerationCmd const & );
