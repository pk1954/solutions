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

int  g_AllocHookCounter = 0;
HWND G_hwndApp;

int main( int argc, char * argv [ ], char * envp [ ] )
{
	PixelCoordinates PixCoords;

	wcout << VER_PRODUCTNAME_STR << L" " << VER_FILE_DESCRIPTION_STR << endl;
	wcout << L"Build at " << __DATE__ << L" " << __TIME__ << endl;

	wofstream m_traceStream = OpenTraceFile( L"main_trace.out" );

    Config::SetDefaultConfiguration( );
    Config::DefineConfigWrapperFunctions( );

	int const iNrOfNeighbors = Config::GetConfigValue( Config::tId::nrOfNeighbors );

	GridDimensions::DefineGridSize( 200_GRID_COORD, 100_GRID_COORD, iNrOfNeighbors );

    Script::ProcessScript( L"std_configuration.in" );

	EvolutionCore::InitClass( iNrOfNeighbors, nullptr, nullptr );

	PIXEL                 const FIELDSIZE            = 8_PIXEL;
	EvoModelDataGlue    * const pEvoModelData        = new EvoModelDataGlue( );
	BOOL                  const bHexagonMode         = (iNrOfNeighbors == 6);
	EvoHistorySysGlue   * const pEvoHistGlue         = new EvoHistorySysGlue( );
    WorkThreadInterface * const pWorkThreadInterface = new WorkThreadInterface( );
    HistorySystem       * const pHistorySystem       = HistorySystem::CreateHistorySystem( );
	EvolutionCore       * const pEvolutionCore       = pEvoModelData->GetEvolutionCore( );

	pWorkThreadInterface->Initialize( & m_traceStream );
	DefineCoreWrapperFunctions( pEvolutionCore );
	DefinePixelCoordinatesWrapperFunctions( & PixCoords );

	PixCoords.Start( FIELDSIZE, bHexagonMode );
	pEvoHistGlue->Start( pHistorySystem, nullptr, FALSE );  // do history cache allocation **not** asynchroniously
    DefineWin32HistWrapperFunctions( pWorkThreadInterface );

	pWorkThreadInterface->Start( nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, pEvoHistGlue );

	DefineWin32WrapperFunctions( pWorkThreadInterface );

    wstring wstrInputFile = L"Test_1.in";

	for ( int iCount = 1; iCount < argc; iCount++ )
    {
        std::string strCmd( argv[ iCount ] );

		if ( (strCmd.find( ".in" ) != string::npos) || (strCmd.find( ".IN" ) != string::npos) ) 
		{
			wstrInputFile.assign( strCmd.begin(), strCmd.end() ); 
		}
    }

	std::wstring wstr( wstrInputFile );
	Script::ProcessScript( wstr );

	wcout << L" ***** EvolutionConsole terminates successfully *****" << endl;

	return 0;
}

