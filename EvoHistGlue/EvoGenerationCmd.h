// EvoGenerationCmd.h
//

#pragma once

#include "generationCmd.h"

enum class tEvoCmd : unsigned short // enumeration starts after last tGenCmd value
{
    editSetBrushMode = static_cast<int>(tGenCmd::GEN_LAST) + 1,
    editSetBrushShape,
    editSetBrushSize,
    editSetBrushIntensity,
    editSetXvalue,
    editSetYvalue
};

class EvoGenerationCmd: public GenerationCmd
{
public:

    EvoGenerationCmd( )
    {
        GenerationCmd( );
    }

    explicit EvoGenerationCmd( tEvoCmd const cmd ) :
        GenerationCmd( static_cast<tGenCmd>( cmd ) )
    { }

    explicit EvoGenerationCmd( tGenCmd const cmd ) :
        GenerationCmd( cmd )
    { }

    EvoGenerationCmd( tEvoCmd const cmd, short const s ) :
        GenerationCmd( static_cast<tGenCmd>( cmd ), s )
    { }

    EvoGenerationCmd( tGenCmd const cmd, short const s ) :
        GenerationCmd( cmd, s )
    { }

    tEvoCmd GetEvoCommand( ) const { return static_cast<tEvoCmd>(GetCommand( )); }
};

wchar_t const * const GetEvoCommandName     ( tEvoCmd const );
wchar_t const * const GetEvoCommandNameShort( tEvoCmd const );

