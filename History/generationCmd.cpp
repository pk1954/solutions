// generationCmd.cpp
//

#include "stdafx.h"
#include <unordered_map>
#include "generationCmd.h"

using namespace std;

wchar_t const * const GetGenerationCmdNameShort( tGenCmd const cmd )
{
    static unordered_map < tGenCmd, wchar_t const * const > mapNames =
    {
        { tGenCmd::cached,    L"CACHE" },
        { tGenCmd::nextGen,   L"NEXT " },
        { tGenCmd::reset,     L"RESET" },
        { tGenCmd::undefined, L"UNDEF" }
    };

    return mapNames.at( cmd );
}

wchar_t const * const GetGenerationCmdName( tGenCmd const cmd )
{
    static unordered_map < tGenCmd, wchar_t const * const > mapNames =
    {
        { tGenCmd::cached,    L"tGenCmd::cached" },
        { tGenCmd::nextGen,   L"tGenCmd::nextGen" },
        { tGenCmd::reset,     L"tGenCmd::reset" },
        { tGenCmd::undefined, L"tGenCmd::undefined" }
    };

    return mapNames.at( cmd );
}

wostream & operator << ( wostream & out, GenerationCmd const & genCmd )
{
    tGenCmd cmd = genCmd.GetCommand( );
    out << GetGenerationCmdNameShort( cmd );
    return out;
};
