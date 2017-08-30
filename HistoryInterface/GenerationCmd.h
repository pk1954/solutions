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
	typedef short tGenCmd;

    GenerationCmd( )
    {
        InitializeCmd( );
    }

    GenerationCmd( tGenCmd const cmd, unsigned short const us ) :
        m_Cmd( cmd ),
        m_usParam( us )
    { }

    GenerationCmd( int const uiCmd, unsigned short const us ) :
        m_Cmd( static_cast<tGenCmd>( uiCmd ) ),
        m_usParam( us )
    { }

    tGenCmd        GetCommand( ) const { return m_Cmd; }
    unsigned short GetParam( )   const { return m_usParam; }

	bool IsDefined( )             const { return m_Cmd != UNDEFINED; }
    bool IsUndefined( )           const { return m_Cmd == UNDEFINED; }
    bool IsCachedGeneration( )    const { return m_Cmd == CACHED; }
    bool IsNotCachedGeneration( ) const { return m_Cmd != CACHED; }

    void InitializeCmd( )
    {
        m_Cmd     = UNDEFINED;
        m_usParam = USHRT_MAX;
    }

	static tGenCmd      const CACHED    =   -1;
	static tGenCmd      const UNDEFINED =    0;
	static unsigned int const FIRST_APP_CMD = 1000;

private:
    tGenCmd        m_Cmd;
    unsigned short m_usParam;
};

wchar_t const * const GetGenerationCmdNameShort( GenerationCmd::tGenCmd const );
wchar_t const * const GetGenerationCmdName     ( GenerationCmd::tGenCmd const );

wostream & operator << ( wostream &, GenerationCmd const & );
