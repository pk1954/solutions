// GenerationCmd.h : 
//

#pragma once

#include "limits.h"
#include "assert.h"
#include <fstream>
#include <iomanip>

using namespace std;

class GenerationCmd
{ 
public:
	typedef int16_t tGenCmd;

    GenerationCmd( )
    {
        InitializeCmd( );
    }

    GenerationCmd( tGenCmd const cmd, int16_t const p ) :
        m_Cmd( cmd ),
        m_Param( p )
    { }

    GenerationCmd( int const uiCmd, int16_t const p ) :
        m_Cmd( static_cast<tGenCmd>( uiCmd ) ),
        m_Param( p )
    { }

    tGenCmd GetCommand( ) const { return m_Cmd; }
    int16_t GetParam( )   const { return m_Param; }

	bool IsDefined( )             const { return m_Cmd != UNDEFINED; }
    bool IsUndefined( )           const { return m_Cmd == UNDEFINED; }
    bool IsCachedGeneration( )    const { return m_Cmd == CACHED; }
    bool IsNotCachedGeneration( ) const { return m_Cmd != CACHED; }
	bool IsAppCommand( )          const { IsAppCmd( m_Cmd ); }

    void InitializeCmd( )
    {
        m_Cmd   = UNDEFINED;
        m_Param = 0xfff;
    }

	static tGenCmd const CACHED        =   -1;
	static tGenCmd const UNDEFINED     =    0;
	static int16_t const FIRST_APP_CMD = 1000;

	static bool IsAppCmd( tGenCmd const cmd ) { return cmd >= FIRST_APP_CMD; }

private:
    tGenCmd m_Cmd;
    int16_t m_Param;
};

wchar_t const * const GetGenerationCmdNameShort( GenerationCmd::tGenCmd const );
wchar_t const * const GetGenerationCmdName     ( GenerationCmd::tGenCmd const );

wostream & operator << ( wostream &, GenerationCmd const & );
