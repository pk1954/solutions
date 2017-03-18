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

class EvoGenerationCmd
{
public:

    EvoGenerationCmd( tEvoCmd const cmd, short const s ) :
        m_Cmd( cmd ),
		m_sParam( s )
    { }

	tEvoCmd GetEvoCommand( ) const { return m_Cmd; }
    short   GetParam( )      const { return m_sParam; }

private:
    tEvoCmd m_Cmd;
    short   m_sParam;
};

wchar_t const * const GetEvoCommandName     ( tEvoCmd const );
wchar_t const * const GetEvoCommandNameShort( tEvoCmd const );
