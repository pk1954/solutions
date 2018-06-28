// EvolutionConsole.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

// EvolutionCore interfaces

#include "config.h"
#include "trace.h"
#include "EvolutionCoreWrappers.h"
#include "EvolutionModelData.h"
#include "EvolutionCore.h"
#include "pixelCoordinates.h"
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

	int   const iNrOfNeighbors = Config::GetConfigValue( Config::tId::nrOfNeighbors );
	BOOL  const bHexagonMode   = (iNrOfNeighbors == 6);
	short const FIELDSIZE      = 8;

	DefinePixelCoordinatesWrapperFunctions( new PixelCoordinates( FIELDSIZE, bHexagonMode ) );

    DefineModelWrapperFunctions( EvolutionModelData::CreateModelData( ) );

    WorkThread * m_pWorkThread = new WorkThread( & m_traceStream );

    wstring strInputFile = L"test.in";

	for ( int iCount = 0; iCount < argc; iCount++ )
    {
        std::string strCmd( argv[ iCount ] );

        if ( strCmd.compare( "/nohist" ) == 0 )
		{
            Config::SetConfigValue( Config::tId::maxGeneration, 0 );
		}
		else if (strCmd.find( ".in" ) != string::npos )
		{
			strInputFile.assign( strCmd.begin(), strCmd.end() ); 
		}
    }

    Script::ProcessScript( strInputFile );

    return 0;
}

