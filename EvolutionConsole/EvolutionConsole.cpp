// EvolutionConsole.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

// EvolutionCore interfaces

#include "config.h"
#include "trace.h"
#include "EvolutionCoreWrappers.h"
#include "EvolutionModelData.h"
#include "EvolutionCore.h"
#include "win32_worker_thread.h"
#include "version.h"

// scripting and tracing

#include "script.h"

using namespace std;

int main( int argc, char * argv [ ], char * envp [ ] )
{
	wcout << VER_PRODUCTNAME_STR << L" " << VER_FILE_DESCRIPTION_STR << endl;
	wcout << L"Build at " << __DATE__ << L" " << __TIME__ << endl;

	wofstream m_traceStream = OpenTraceFile( L"main_trace.out" );

    Config::SetDefaultConfiguration( );
    Config::DefineConfigWrapperFunctions( );

    Script::ProcessScript( L"std_configuration.in" );

	EvolutionCore::InitClass( );
    EvolutionCore::CreateCore( );

    EvolutionModelData * m_pModelWork = EvolutionModelData::CreateModelData( );
    DefineModelWrapperFunctions( m_pModelWork );

    WorkThread * m_pWorkThread = new WorkThread( & m_traceStream );

    for ( int iCount = 0; iCount < argc; iCount++ )
    {
        std::string const strCmdLine( argv[ iCount ] );

        if ( strCmdLine.compare( "/nohist" ) == 0 )
            Config::SetConfigValue( Config::tId::maxGeneration, 0 );
    }

    Script::ProcessScript( L"std_script.in" );

    return 0;
}

