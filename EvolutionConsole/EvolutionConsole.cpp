// EvolutionConsole.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

// EvolutionCore interfaces

#include "config.h"
#include "trace.h"
#include "dump.h"
#include "GridDimensions.h"
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

HWND G_hwndApp;

int main( int argc, char * argv [ ], char * envp [ ] )
{
	wcout << VER_PRODUCTNAME_STR << L" " << VER_FILE_DESCRIPTION_STR << endl;
	wcout << L"Build at " << __DATE__ << L" " << __TIME__ << endl;

	wofstream m_traceStream = OpenTraceFile( L"main_trace.out" );

    Config::SetDefaultConfiguration( );
    Config::DefineConfigWrapperFunctions( );
	GridDimensions::DefineGridSize( 200_GRID_COORD, 100_GRID_COORD );

    Script::ProcessScript( L"std_configuration.in" );

	PIXEL                 const FIELDSIZE            = 8_PIXEL;
	int                   const iNrOfNeighbors       = Config::GetConfigValue( Config::tId::nrOfNeighbors );
    EvolutionCore       * const pEvolutionCore       = EvolutionCore::InitClass( iNrOfNeighbors, nullptr, nullptr );
	BOOL                  const bHexagonMode         = (iNrOfNeighbors == 6);
	PixelCoordinates    * const pPixCoords           = new PixelCoordinates( FIELDSIZE, bHexagonMode );
	EvoHistorySysGlue   * const pEvoHistGlue         = new EvoHistorySysGlue( );
    WorkThreadInterface * const pWorkThreadInterface = new WorkThreadInterface( & m_traceStream );
    HistorySystem       * const pHistorySystem       = HistorySystem::CreateHistorySystem( );

	DefinePixelCoordinatesWrapperFunctions( pPixCoords );

	pEvoHistGlue->Start( pEvolutionCore, pHistorySystem, false, nullptr );
    DefineWin32HistWrapperFunctions( pWorkThreadInterface );

	pWorkThreadInterface->Start( nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, pEvolutionCore, pEvoHistGlue );

	DefineWin32WrapperFunctions( pWorkThreadInterface );

    wstring wstrInputFile = L"Test_4.in";

	for ( int iCount = 1; iCount < argc; iCount++ )
    {
        std::string strCmd( argv[ iCount ] );

		if ( (strCmd.find( ".in" ) != string::npos) || (strCmd.find( ".IN" ) != string::npos) ) 
		{
			wstrInputFile.assign( strCmd.begin(), strCmd.end() ); 
		}
    }

	pWorkThreadInterface->DoProcessScript( new wstring( wstrInputFile ) );

	wcout << L" ***** EvolutionConsole terminates successfully *****" << endl;

	return 0;
}

