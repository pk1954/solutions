// EvolutionConsole.cpp : Defines the entry point for the console application.
//

import std;
import RunTime;
import Trace;
import EvoCoreLib;
import EvoHistorySysGlue;
import win32_EvoWorkThreadInterface;
import win32_wrappers;
import win32_histWrappers;
import version;

using std::string;
using std::wstring;
using std::wofstream;
using std::endl;

HWND G_hwndApp;

int main(int argc, char * argv [], char * envp [])
{
	EvoPixelCoords PixCoords;

	wcout << VER_PRODUCTNAME_STR << L" " << VER_FILE_DESCRIPTION_STR << endl;
	wcout << L"Build at " << __DATE__ << L" " << __TIME__ << endl;

	wofstream m_traceStream = OpenTraceFile(L"main_trace.out");

    EvoConfig::SetDefaultConfiguration();
    EvoConfig::DefineConfigWrapperFunctions();

	int const iNrOfNeighbors = EvoConfig::GetConfigValue(EvoConfig::tId::nrOfNeighbors);

	GridDimensions::DefineGridSize(200_GRID_COORD, 100_GRID_COORD, iNrOfNeighbors);

    Script script;
	script.ScrProcess(L"std_configuration.in");

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
	pEvoHistGlue->Start(pHistorySystem, false);  // do history cache allocation **not** asynchroniously
    DefineWin32HistWrapperFunctions(pWorkThreadInterface);

	pWorkThreadInterface->Start(nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, pEvoHistGlue);

	DefineWin32WrapperFunctions(pWorkThreadInterface);

    wstring wstrInputFile = L"Test_1.in";

	for (int iCount = 1; iCount < argc; iCount++)
    {
        string strCmd(argv[iCount]);

		if ((strCmd.find(".in") != string::npos) || (strCmd.find(".IN") != string::npos)) 
		{
			wstrInputFile.assign(strCmd.begin(), strCmd.end()); 
		}
    }

	wstring wstr(wstrInputFile);
	Script script;
	script.ScrProcess(wstr);

	wcout << L" ***** EvolutionConsole terminates successfully *****" << endl;

	return 0;
}

