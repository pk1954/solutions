// EvolutionConsole.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

// EvolutionCore interfaces

#include "config.h"
#include "trace.h"
#include "dump.h"
#include "EvoHistorySysGlue.h"
#include "EvolutionCoreWrappers.h"
#include "EvolutionCore.h"
#include "pixelCoordinates.h"
#include "win32_workThreadInterface.h"
#include "win32_wrappers.h"
#include "win32_histWrappers.h"
#include "win32_util.h"
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

    WorkThreadInterface * m_pWorkThreadInterface;
	EvoHistorySysGlue   * m_pEvoHistGlue;
	
	m_pEvoHistGlue         = new EvoHistorySysGlue( );
	m_pWorkThreadInterface = new WorkThreadInterface( & m_traceStream );
	m_pEvoHistGlue->Start( m_pEvolutionCore, Util::GetMaxNrOfSlots( EvolutionCore::GetModelSize( ) ), false );
    DefineWin32HistWrapperFunctions( m_pWorkThreadInterface );

	m_pWorkThreadInterface->Start( nullptr, nullptr, nullptr, m_pEvolutionCore, m_pEvoHistGlue );

	DefineWin32WrapperFunctions( m_pWorkThreadInterface, nullptr );

    wstring wstrInputFile = L"Test_4.in";

	for ( int iCount = 1; iCount < argc; iCount++ )
    {
        std::string strCmd( argv[ iCount ] );

		if ( (strCmd.find( ".in" ) != string::npos) || (strCmd.find( ".IN" ) != string::npos) ) 
		{
			wstrInputFile.assign( strCmd.begin(), strCmd.end() ); 
		}
    }

	m_pWorkThreadInterface->DoProcessScript( new wstring( wstrInputFile ) );

	wcout << L" ***** EvolutionConsole terminates successfully *****" << endl;

	return 0;
}

