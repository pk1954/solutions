// generationCmd.cpp
//

#include "stdafx.h"
#include <unordered_map>
#include "GenerationCmd.h"

using namespace std;

wchar_t const * const GetGenerationCmdNameShort( GenerationCmd::tGenCmd const cmd )
{
    static unordered_map < GenerationCmd::tGenCmd, wchar_t const * const > mapNames =
    {
        { GenerationCmd::CACHED,    L"CACHE" },
        { GenerationCmd::UNDEFINED, L"UNDEF" }
    };

    return mapNames.at( cmd );
}

wchar_t const * const GetGenerationCmdName( GenerationCmd::tGenCmd const cmd )
{
    static unordered_map < GenerationCmd::tGenCmd, wchar_t const * const > mapNames =
    {
        { GenerationCmd::CACHED,    L"tGenCmd::cached" },
        { GenerationCmd::UNDEFINED, L"tGenCmd::undefined" }
    };

    return mapNames.at( cmd );
}

wostream & operator << ( wostream & out, GenerationCmd const & genCmd )
{
    GenerationCmd::tGenCmd cmd = genCmd.GetCommand( );
    out << GetGenerationCmdNameShort( cmd );
    return out;
};
