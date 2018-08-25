// GenerationCmd.h : 
//
// GenerationCmd stores all necessary information to transform a model from generation n to generation n+1
// The size of GenerationCmd is 32 bit

#pragma once

#include <fstream>
#include <iomanip>
#include "limits.h"
#include "assert.h"
#include "debug.h"

using namespace std;

enum class tGenCmd : int8_t
{
	UNDEFINED,
	CACHED,
	FIRST_APP_CMD
};

class GenerationCmd
{ 
public:
    GenerationCmd( )
    {
        InitializeCmd( );
    }

    tGenCmd GetCommand( ) const { return m_Cmd; }
    int16_t GetParam( )   const { return m_Param; }

	bool IsDefined( )             const { return m_Cmd != tGenCmd::UNDEFINED; }
    bool IsUndefined( )           const { return m_Cmd == tGenCmd::UNDEFINED; }
    bool IsCachedGeneration( )    const { return m_Cmd == tGenCmd::CACHED; }
    bool IsNotCachedGeneration( ) const { return m_Cmd != tGenCmd::CACHED; }
	bool IsAppCommand( )          const { IsAppCmd( m_Cmd ); }

    void InitializeCmd( )
    {
        m_Cmd   = tGenCmd::UNDEFINED;
        m_Param = 0xfff;
    }

 	static int const MAX_APP_CMD = 0xff;

	static bool IsAppCmd( tGenCmd const cmd ) { return cmd >= tGenCmd::FIRST_APP_CMD; }

    static GenerationCmd CachedCmd( short const sSlotNr )
    {
		return GenerationCmd( tGenCmd::CACHED, sSlotNr );
    }

    static GenerationCmd ApplicationCmd( tGenCmd cmd, int16_t const param )
    {
		ASSERT_LIMITS( static_cast<int>(cmd), static_cast<int>(tGenCmd::FIRST_APP_CMD), MAX_APP_CMD );
		return GenerationCmd( cmd, param );
    }

private:
    GenerationCmd( tGenCmd const cmd, int16_t const p ) :
        m_Cmd( cmd ),
        m_Param( p )
    { }

    tGenCmd m_Cmd;
    int16_t m_Param;
};

wchar_t const * const GetGenerationCmdNameShort( tGenCmd const );
wchar_t const * const GetGenerationCmdName     ( tGenCmd const );

wostream & operator << ( wostream &, GenerationCmd const & );
