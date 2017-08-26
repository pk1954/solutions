// EvoGenerationCmd.cpp
//

#include "stdafx.h"
#include <unordered_map>
#include <fstream>
#include "EvoGenerationCmd.h"

using namespace std;

wchar_t const * const GetEvoCommandNameShort( tEvoCmd const cmd )
{
    static unordered_map < tEvoCmd, wchar_t const * const > mapNames =
    {
        { tEvoCmd::editSetBrushMode,      L"MODE" },
        { tEvoCmd::editSetBrushShape,     L"SHAPE" },
        { tEvoCmd::editSetBrushSize,      L"SIZE" },
        { tEvoCmd::editSetBrushIntensity, L"INTENS" },
        { tEvoCmd::editDoEdit,            L"EDIT" }
    };

    return mapNames.at( cmd );
}

wchar_t const * const GetEvoCommandName( tEvoCmd const cmd )
{
    static unordered_map < tEvoCmd, wchar_t const * const > mapNames =
    {
        { tEvoCmd::editSetBrushMode,      L"tEvoCmd::editSetBrushMode" },
        { tEvoCmd::editSetBrushShape,     L"tEvoCmd::editSetBrushShape" },
        { tEvoCmd::editSetBrushSize,      L"tEvoCmd::editSetBrushSize" },
        { tEvoCmd::editSetBrushIntensity, L"tEvoCmd::editSetBrushIntensity" },
        { tEvoCmd::editDoEdit,            L"tEvoCmd::editDoEdit" }
    };

    return mapNames.at( cmd );
}
