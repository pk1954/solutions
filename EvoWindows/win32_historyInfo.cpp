// win32_historyInfo.cpp :
//

#include "stdafx.h"
#include "resource.h"
#include "win32_historyInfo.h"
#include <fstream>
#include <iostream>
#include <sstream> 
#include <locale>
#include "HistoryGeneration.h"
#include "HistorySystem.h"

void ShowHistoryInfo( HistorySystem const & histSys )
{
    long long lNrOfSlots = histSys.GetNrOfHistCacheSlots( );
    long long lSlotSize  = histSys.GetSlotSize( );
    long long lTotalSize = lNrOfSlots * lSlotSize;
    std::wostringstream wBuffer;
    wBuffer.imbue(std::locale(""));
    wBuffer << L"# of slots  " << lNrOfSlots << endl;
    wBuffer << L"slot size   " << lSlotSize  << endl;
    wBuffer << L"total size  " << lTotalSize << endl;
    wBuffer << L"genCurrent  " << histSys.GetCurrentGeneration( )  << endl;
	wBuffer << L"genYoungest " << histSys.GetYoungestGeneration( ) << endl;
    MessageBox( nullptr, wBuffer.str( ).c_str( ), L"History System Information", MB_OK | MB_SYSTEMMODAL );
}
