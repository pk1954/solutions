// EvoGenerationCmd.h
//

#pragma once

#include "generationCmd.h"

enum class tEvoCmd : unsigned short // enumeration starts after last tGenCmd value
{
    editSetBrushMode = static_cast<int>(tGenCmd::last) + 1,
    editSetBrushShape,
    editSetBrushSize,
    editSetBrushIntensity,
    editSetXvalue,
    editSetYvalue
};

class EvoGenerationCmd: public GenerationCmd
{
public:

    EvoGenerationCmd( tEvoCmd const cmd, short const s ) :
        GenerationCmd( static_cast<tGenCmd>( cmd ), s )
    { }


    tEvoCmd GetEvoCommand( ) const { return static_cast<tEvoCmd>(GetCommand( )); }
};

wchar_t const * const GetEvoCommandName     ( tEvoCmd const );
wchar_t const * const GetEvoCommandNameShort( tEvoCmd const );

