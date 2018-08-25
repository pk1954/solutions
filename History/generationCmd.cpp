// generationCmd.cpp
//

#include "stdafx.h"
#include <unordered_map>
#include "GenerationCmd.h"

using namespace std;

wchar_t const * const GetGenerationCmdNameShort( tGenCmd const cmd )
{
    static unordered_map < tGenCmd, wchar_t const * const > mapNames =
    {
        { tGenCmd::CACHED,    L"CACHE" },
        { tGenCmd::UNDEFINED, L"UNDEF" }
    };

    return mapNames.at( cmd );
}

wchar_t const * const GetGenerationCmdName( tGenCmd const cmd )
{
    static unordered_map < tGenCmd, wchar_t const * const > mapNames =
    {
        { tGenCmd::CACHED,    L"tGenCmd::cached" },
        { tGenCmd::UNDEFINED, L"tGenCmd::undefined" }
    };

    return mapNames.at( cmd );
}

wostream & operator << ( wostream & out, GenerationCmd const & genCmd )
{
    tGenCmd cmd = genCmd.GetCommand( );
    out << GetGenerationCmdNameShort( cmd );
    return out;
};
