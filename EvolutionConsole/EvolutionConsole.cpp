// EvolutionConsole.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

// EvolutionCore interfaces

#include "config.h"
#include "trace.h"
#include "dump.h"
#include "EvoHistorySys.h"
#include "EvolutionModelData.h"
#include "EvolutionCoreWrappers.h"
#include "EvolutionCore.h"
#include "pixelCoordinates.h"
#include "win32_status.h"
#include "win32_histWorkerThread.h"
#include "win32_worker_thread.h"
#include "win32_wrappers.h"
#include "win32_histWrappers.h"
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
    EvolutionCore * m_pEvolutionCore = EvolutionCore::CreateCore( );

	int                const iNrOfNeighbors = Config::GetConfigValue( Config::tId::nrOfNeighbors );
	BOOL               const bHexagonMode   = (iNrOfNeighbors == 6);
	short              const FIELDSIZE      = 8;
	PixelCoordinates * const pPixCoords     = new PixelCoordinates( FIELDSIZE, bHexagonMode );

	DefinePixelCoordinatesWrapperFunctions( pPixCoords );

    WorkThread         * m_pWorkThread;
	EvoHistorySys      * m_pEvoHistorySys;
    HistWorkThread     * m_pHistWorkThread;
    EvolutionModelData * m_pModelWork;

	m_pModelWork = EvolutionModelData::CreateModelData( );
    DefineModelWrapperFunctions( EvolutionModelData::CreateModelData( ) );
	
	if ( Config::UseHistorySystem( ) )
    {
		m_pEvoHistorySys  = new EvoHistorySys( );
		m_pHistWorkThread = new HistWorkThread( & m_traceStream, m_pModelWork, m_pEvoHistorySys, nullptr );
		m_pWorkThread     = m_pHistWorkThread;
		m_pEvoHistorySys->Start( m_pModelWork, m_pHistWorkThread, nullptr, EvolutionCore::GetModelSize( ), false );
        DefineWin32HistWrapperFunctions( m_pHistWorkThread );
    }
    else
    {
        m_pWorkThread = new WorkThread( & m_traceStream );
    }

	m_pWorkThread->Start( nullptr, nullptr, nullptr, nullptr, m_pEvolutionCore, m_pModelWork );

	DefineWin32WrapperFunctions( m_pHistWorkThread, m_pWorkThread, nullptr );

    wstring wstrInputFile = L"Test_3.in";

	for ( int iCount = 1; iCount < argc; iCount++ )
    {
        std::string strCmd( argv[ iCount ] );

        if ( strCmd.compare( "/nohist" ) == 0 )
		{
            Config::SetConfigValue( Config::tId::maxGeneration, 0 );
		}
		else if ( (strCmd.find( ".in" ) != string::npos) || (strCmd.find( ".IN" ) != string::npos) ) 
		{
			wstrInputFile.assign( strCmd.begin(), strCmd.end() ); 
		}
    }

	m_pWorkThread->DoProcessScript( new wstring( wstrInputFile ) );

	wcout << L" ***** EvolutionConsole terminates successfully *****" << endl;

	return 0;
}

