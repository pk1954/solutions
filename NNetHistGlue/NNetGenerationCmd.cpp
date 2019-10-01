// NNetGenerationCmd.cpp
//
// NNetHistGlue

#include "stdafx.h"
#include <unordered_map>
#include <fstream>
#include "NNetGenerationCmd.h"

wchar_t const * const GetNNetCommandNameShort( NNetGenerationCmd::Id const cmd )
{
    static std::unordered_map < NNetGenerationCmd::Id, wchar_t const * const > mapNames =
    {
		{ NNetGenerationCmd::Id::highlight, L"HIGHLIGHT" },
		{ NNetGenerationCmd::Id::editLast,  L"EDIT_2" } 
	};

    return mapNames.at( cmd );
}

wchar_t const * const GetNNetCommandName( NNetGenerationCmd::Id const cmd )
{
    static std::unordered_map < NNetGenerationCmd::Id, wchar_t const * const > mapNames =
    {
		{ NNetGenerationCmd::Id::highlight, L"GenerationCmd::Id::highlight" },
		{ NNetGenerationCmd::Id::editLast,  L"GenerationCmd::Id::editLast"  }
	};

    return mapNames.at( cmd );
}

std::wostream & operator << ( std::wostream & out, NNetGenerationCmd::Id const & cmd )
{
	if ( GenerationCmd::IsAppCmd( static_cast<GenerationCmd::Id>(cmd) ) )
		out << GetNNetCommandNameShort( cmd  );
	else 
		out << GetGenerationCmdNameShort( static_cast<GenerationCmd::Id>(cmd) );
    return out;
};
