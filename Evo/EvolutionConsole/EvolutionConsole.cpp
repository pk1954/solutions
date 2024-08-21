// EvolutionConsole.cpp : Defines the entry point for the console application.
//


// EvolutionCore interfaces

import EvolutionDump;
#include "EvoHistorySysGlue.h"
#include "EvolutionCoreWrappers.h"
#include "EvolutionCore.h"
#include "EvoPixelCoords.h"
#include "win32_EvoWorkThreadInterface.h"
#include "win32_wrappers.h"
#include "win32_histWrappers.h"
#include "version.h"

// scripting and tracing

import RunTime;
import Trace;
import Config;
import GridDimensions;

using namespace std;

HWND G_hwndApp;

int main(int argc, char * argv [], char * envp [])
{
	EvoPixelCoords PixCoords;

	wcout << VER_PRODUCTNAME_STR << L" " << VER_FILE_DESCRIPTION_STR << endl;
	wcout << L"Build at " << __DATE__ << L" " << __TIME__ << endl;

	wofstream m_traceStream = OpenTraceFile(L"main_trace.out");

    Config::SetDefaultConfiguration();
    Config::DefineConfigWrapperFunctions();

	int const iNrOfNeighbors = Config::GetConfigValue(Config::tId::nrOfNeighbors);

	GridDimensions::DefineGridSize(200_GRID_COORD, 100_GRID_COORD, iNrOfNeighbors);

    RunTime::ProcessScript(L"std_configuration.in");

	EvolutionCore::InitClass(iNrOfNeighbors, nullptr, nullptr);

	PIXEL                    const FIELDSIZE            = 8_PIXEL;
	EvoModelDataGlue       * const pEvoModelData        = new EvoModelDataGlue();
	bool                     const bHexagonMode         = (iNrOfNeighbors == 6);
	EvoHistorySysGlue      * const pEvoHistGlue         = new EvoHistorySysGlue();
    EvoWorkThreadInterface * const pWorkThreadInterface = new EvoWorkThreadInterface();
    HistorySystem          * const pHistorySystem       = HistorySystem::CreateHistorySystem();
	EvolutionCore          * const pEvolutionCore       = pEvoModelData->GetEvolutionCore();

	pWorkThreadInterface->Initialize(& m_traceStream);
	DefineCoreWrapperFunctions(pEvolutionCore);
	DefineEvoPixelCoordsWrapperFunctions(& PixCoords);

	PixCoords.Start(FIELDSIZE, bHexagonMode);
	pEvoHistGlue->Start(pHistorySystem, FALSE);  // do history cache allocation **not** asynchroniously
    DefineWin32HistWrapperFunctions(pWorkThreadInterface);

	pWorkThreadInterface->Start(nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, pEvoHistGlue);

	DefineWin32WrapperFunctions(pWorkThreadInterface);

    wstring wstrInputFile = L"Test_1.in";

	for (int iCount = 1; iCount < argc; iCount++)
    {
        std::string strCmd(argv[iCount]);

		if ((strCmd.find(".in") != string::npos) || (strCmd.find(".IN") != string::npos)) 
		{
			wstrInputFile.assign(strCmd.begin(), strCmd.end()); 
		}
    }

	std::wstring wstr(wstrInputFile);
	RunTime::ProcessScript(wstr);

	wcout << L" ***** EvolutionConsole terminates successfully *****" << endl;

	return 0;
}

