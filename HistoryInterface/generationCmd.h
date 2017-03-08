// generationCmd.h : 
//

#pragma once

#include "limits.h"
#include <fstream>
#include <iomanip>

using namespace std;

enum class tGenCmd : unsigned short
{
    undefined,
    cached,
    nextGen,
    reset,
    GEN_LAST = reset
};

class GenerationCmd
{ 
public:
    GenerationCmd( )
    {
        InitializeCmd( );
    }

    explicit GenerationCmd( tGenCmd const cmd ) :
        m_Cmd( cmd ),
        m_sParam( SHRT_MAX )
    { }

    GenerationCmd( tGenCmd const cmd, short const s ) :
        m_Cmd( cmd ),
        m_sParam( s )
    { }

    tGenCmd GetCommand( ) const { return m_Cmd; }
    short   GetParam( )   const { return m_sParam; }

    bool IsDefined( )             const { return m_Cmd != tGenCmd::undefined; }
    bool IsUndefined( )           const { return m_Cmd == tGenCmd::undefined; }
    bool IsCachedGeneration( )    const { return m_Cmd == tGenCmd::cached; }
    bool IsNotCachedGeneration( ) const { return m_Cmd != tGenCmd::cached; }

    void InitializeCmd( )
    {
        m_Cmd    = tGenCmd::undefined;
        m_sParam = SHRT_MAX;
    }

private:
    tGenCmd m_Cmd;
    short  m_sParam;
};

static const GenerationCmd GEN_CMD_UNDEFINED = { tGenCmd::undefined, SHRT_MAX };
static const GenerationCmd GEN_CMD_NEXT_GEN  = { tGenCmd::nextGen,  0 };
static const GenerationCmd GEN_CMD_RESET     = { tGenCmd::reset,     0 };

wchar_t const * const GetGenerationCmdNameShort( tGenCmd const );
wchar_t const * const GetGenerationCmdName     ( tGenCmd const );

wostream & operator << ( wostream &, GenerationCmd const & );
