// win32_NNetSimuController.h
//
// NNetSimu

#pragma once

class NNetWorkThreadInterface;
class NNetAppWindow;
class NNetAppMenu;
class WinManager;
class StatusBar;
class Delay;

class NNetSimuController
{
public:
	NNetSimuController
	( 
		WinManager * const
	);

	virtual ~NNetSimuController( );

	void Initialize
	( 
		NNetAppWindow           * const,
		NNetWorkThreadInterface * const,
		Delay                   * const
	);

	bool ProcessUIcommand   ( int const, LPARAM const );
	bool ProcessModelCommand( int const, LPARAM const );

private:

	NNetAppWindow          * m_pAppWindow;
	NNetWorkThreadInterface * m_pNNetWorkThreadInterface;
	WinManager              * m_pWinManager;
	Delay                   * m_pDelay;
	StatusBar               * m_pStatusBar;
	HCURSOR                   m_hCrsrWait;
};				          
