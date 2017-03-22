// EvoGenerationCmd.h
//

#pragma once

enum class tEvoCmd : unsigned short // enumeration starts after last tGenCmd value
{
    editSetBrushMode,
    editSetBrushShape,
    editSetBrushSize,
    editSetBrushIntensity,
    editSetXvalue,
    editSetYvalue
};

wchar_t const * const GetEvoCommandName     ( tEvoCmd const );
wchar_t const * const GetEvoCommandNameShort( tEvoCmd const );
