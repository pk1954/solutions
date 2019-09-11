// win32_NNetSimuController.h
//

#pragma once

#include <fstream>

class NNetSimuWindow;
class Delay;
class StatusBar;
class WinManager;
class NNetSimuMenu;

class NNetSimuController
{
public:
	NNetSimuController( );

	virtual ~NNetSimuController( );

	void Initialize
	( 
		NNetSimuWindow * const,
		std::wostream  *,
		WinManager     * const,
		Delay          * const,
		NNetSimuMenu   * const
	);

	void ProcessCommand( WPARAM const, LPARAM const = 0 );

private:
	bool processUIcommand( int const, LPARAM const );

	BOOL             m_bTrace;
    std::wostream  * m_pTraceStream;
	NNetSimuWindow * m_pAppWindow;
	WinManager     * m_pWinManager;
	Delay          * m_pDelay;
	StatusBar      * m_pStatusBar;
	NNetSimuMenu   * m_pAppMenu;
	HCURSOR          m_hCrsrWait;
};
