// generationCmd.cpp
//

#include "stdafx.h"
#include <unordered_map>
#include "GenerationCmd.h"

using namespace std;

const GenerationCmd GenerationCmd::UNDEFINED = { tGenCmd::undefined, USHRT_MAX };
const GenerationCmd GenerationCmd::NEXT_GEN  = { tGenCmd::nextGen,   0 };
const GenerationCmd GenerationCmd::RESET     = { tGenCmd::reset,     0 };

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
