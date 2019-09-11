// NNetGenerationCmd.cpp
//
// NNetHistGlue

#include "stdafx.h"
#include <unordered_map>
#include <fstream>
#include "NNetGenerationCmd.h"

bool IsEditorCommand( tNNetCmd const cmd )
{
	static_assert( (int)tNNetCmd::LAST_APP_CMD < GenerationCmd::MAX_APP_CMD, "Too many tNNetCmd values" );

	return ( tNNetCmd::FIRST_EDIT_CMD <= cmd ) && ( cmd <= tNNetCmd::LAST_APP_CMD );
}

wchar_t const * const GetNNetCommandNameShort( tNNetCmd const cmd )
{
    static std::unordered_map < tNNetCmd, wchar_t const * const > mapNames =
    {
        { tNNetCmd::nextGen, L"NEXT"  },
        { tNNetCmd::reset,   L"RESET" } 
    };

    return mapNames.at( cmd );
}

wchar_t const * const GetNNetCommandName( tNNetCmd const cmd )
{
    static std::unordered_map < tNNetCmd, wchar_t const * const > mapNames =
    {
        { tNNetCmd::nextGen, L"tNNetCmd::nextGen" },
        { tNNetCmd::reset,   L"tGenCmd::reset"    }
    };

    return mapNames.at( cmd );
}

std::wostream & operator << ( std::wostream & out, tNNetCmd const & cmd )
{
	if ( GenerationCmd::IsAppCmd( static_cast<tGenCmd>(cmd) ) )
		out << GetNNetCommandNameShort( cmd  );
	else 
		out << GetGenerationCmdNameShort( static_cast<tGenCmd>(cmd) );
    return out;
};
