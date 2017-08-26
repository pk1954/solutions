// generationCmd.h : 
//

#pragma once

#include "limits.h"
#include "assert.h"
#include <fstream>
#include <iomanip>

using namespace std;

enum class tGenCmd : unsigned short
{
    undefined,
    cached,
    nextGen,
    reset,
    last = reset
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
        m_usParam( USHRT_MAX )
    { }

    GenerationCmd( tGenCmd const cmd, unsigned short const us ) :
        m_Cmd( cmd ),
        m_usParam( us )
    { }

    GenerationCmd( unsigned int const uiCmd, unsigned short const us ) :
        m_Cmd( static_cast<tGenCmd>( uiCmd + FIRST_APP_CMD ) ),
        m_usParam( us )
    { }

    tGenCmd        GetCommand( ) const { return m_Cmd; }
    unsigned short GetParam( )   const { return m_usParam; }

    unsigned short GetAppCmd( ) const 
	{ 
		assert( static_cast<unsigned short>(m_Cmd) >= FIRST_APP_CMD ); 
		return  static_cast<unsigned short>(m_Cmd)  - FIRST_APP_CMD; 
	}
		
	bool IsDefined( )             const { return m_Cmd != tGenCmd::undefined; }
    bool IsUndefined( )           const { return m_Cmd == tGenCmd::undefined; }
    bool IsCachedGeneration( )    const { return m_Cmd == tGenCmd::cached; }
    bool IsNotCachedGeneration( ) const { return m_Cmd != tGenCmd::cached; }

    void InitializeCmd( )
    {
        m_Cmd     = tGenCmd::undefined;
        m_usParam = USHRT_MAX;
    }

	static const GenerationCmd UNDEFINED;
	static const GenerationCmd NEXT_GEN;
	static const GenerationCmd RESET;

private:
	static unsigned int const FIRST_APP_CMD = static_cast<unsigned short>(tGenCmd::last) + 1;

    tGenCmd        m_Cmd;
    unsigned short m_usParam;
};

wchar_t const * const GetGenerationCmdNameShort( tGenCmd const );
wchar_t const * const GetGenerationCmdName     ( tGenCmd const );

wostream & operator << ( wostream &, GenerationCmd const & );
