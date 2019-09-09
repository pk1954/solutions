// GenerationCmd.h : 
//
// GenerationCmd stores all necessary information to transform a model from generation n to generation n+1
// The size of GenerationCmd is 32 bit

#pragma once

#include <fstream>
#include <iomanip>
#include <bitset>
#include "limits.h"
#include "assert.h"
#include "debug.h"
#include "Int24.h"
#include "util.h"
#include "HistSlotNr.h"

enum class tGenCmd : int8_t
{
	UNDEFINED,
	CACHED,
	NEXT_GEN,
	FIRST_APP_CMD
};

class GenerationCmd
{ 
public:
    GenerationCmd( )
	   : m_Cmd( tGenCmd::UNDEFINED )
    { }

    tGenCmd    GetCommand( ) const { return m_Cmd; }
	Int24      GetParam  ( ) const { return m_Param; }
	HistSlotNr GetSlotNr ( ) const
	{
		assert( m_Cmd == tGenCmd::CACHED );
		return HistSlotNr{ m_Param.GetValue( ) };
	}

	bool IsDefined( )             const { return m_Cmd != tGenCmd::UNDEFINED; }
    bool IsUndefined( )           const { return m_Cmd == tGenCmd::UNDEFINED; }
    bool IsCachedGeneration( )    const { return m_Cmd == tGenCmd::CACHED; }
    bool IsNotCachedGeneration( ) const { return m_Cmd != tGenCmd::CACHED; }
	bool IsAppCommand( )          const { IsAppCmd( m_Cmd ); }

    void InitializeCmd( )
    {
        m_Cmd = tGenCmd::UNDEFINED;
    }

 	static int const MIN_APP_CMD = static_cast<int>(tGenCmd::FIRST_APP_CMD);
 	static int const MAX_APP_CMD = 0xff;

	static bool IsAppCmd( tGenCmd const cmd ) { return cmd >= tGenCmd::FIRST_APP_CMD; }

	static GenerationCmd const NEXT_GEN_CMD;

	static GenerationCmd CachedCmd( HistSlotNr const slotNr )
    {
		return GenerationCmd( tGenCmd::CACHED, slotNr.GetValue() );
    }

    static GenerationCmd ApplicationCmd( tGenCmd cmd, Int24 const param )
    {
		AssertLimits( static_cast<int>(cmd), MIN_APP_CMD, MAX_APP_CMD );
		return GenerationCmd( cmd, param );
    }

private:
    GenerationCmd( tGenCmd const cmd, Int24 const p ) :
        m_Cmd( cmd ),
		m_Param( p )
	{ }

    tGenCmd m_Cmd;
	Int24   m_Param;
};

wchar_t const * const GetGenerationCmdNameShort( tGenCmd const );
wchar_t const * const GetGenerationCmdName     ( tGenCmd const );

std::wostream & operator << ( std::wostream &, GenerationCmd const & );
