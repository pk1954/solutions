// EvoController.h
//

#pragma once

#include <string>
#include <fstream>

using namespace std;

class PerformanceWindow;
class StatusBar;
class HistWorkThread;
class WinManager;
class GridWindow;

class EvoController
{
public:
	EvoController( );

	virtual ~EvoController( );

	void EvoController::Start
	( 
		wostream          *, 
		HistWorkThread    * const,
		WinManager        * const,
		PerformanceWindow * const,
		StatusBar         * const,
		GridWindow        * const
	);

	void SetGenerationDelay( DWORD const );
	void ProcessCommand( WPARAM const, LPARAM const );

private:
	void setSimulationMode( tBoolOp const );

	BOOL                m_bTrace;
    wostream          * m_pTraceStream;
	HistWorkThread    * m_pHistWorkThread;
	WinManager        * m_pWinManager;
    PerformanceWindow * m_pPerformanceWindow;
	StatusBar         * m_pStatusBar;
	GridWindow        * m_pGridWindow;
};
