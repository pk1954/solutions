// generationCmd.cpp
//
// HistoryInterface

#include "stdafx.h"
#include <unordered_map>
#include "GenerationCmd.h"

GenerationCmd const GenerationCmd::NEXT_GEN_CMD = GenerationCmd(tGenCmd::NEXT_GEN, 0 );

wchar_t const * const GetGenerationCmdNameShort( tGenCmd const cmd )
{
    static std::unordered_map < tGenCmd, wchar_t const * const > mapNames =
    {
		{ tGenCmd::CACHED,    L"CACHE" },
		{ tGenCmd::NEXT_GEN,  L"NEXT"  },
        { tGenCmd::UNDEFINED, L"UNDEF" },
	    { tGenCmd::RESET,     L"RESET" }
	};

    return mapNames.at( cmd );
}

wchar_t const * const GetGenerationCmdName( tGenCmd const cmd )
{
    static std::unordered_map < tGenCmd, wchar_t const * const > mapNames =
    {
	    { tGenCmd::CACHED,    L"tGenCmd::cached"    },
		{ tGenCmd::NEXT_GEN,  L"tGenCmd::nextGen"   },
		{ tGenCmd::UNDEFINED, L"tGenCmd::undefined" },
		{ tGenCmd::RESET,     L"tGenCmd::reset"     }
	};

    return mapNames.at( cmd );
}

std::wostream & operator << ( std::wostream & out, GenerationCmd const & genCmd )
{
    tGenCmd cmd = genCmd.GetCommand( );
    out << GetGenerationCmdNameShort( cmd );
    return out;
};
