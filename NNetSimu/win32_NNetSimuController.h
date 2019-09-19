// win32_NNetSimuController.h
//
// NNetSimu

#pragma once

#include "win32_controller.h"

class NNetWorkThreadInterface;
class NNetSimuWindow;
class NNetAppMenu;
class WinManager;
class StatusBar;
class Delay;

class NNetSimuController : public Controller
{
public:
	NNetSimuController( );

	virtual ~NNetSimuController( );

	void Initialize
	( 
		NNetSimuWindow          * const,
		NNetWorkThreadInterface * const,
		WinManager              * const,
		Delay                   * const
	);

	virtual void ProcessAppCommand( WPARAM const, LPARAM const = 0 );

private:
	bool processUIcommand( int const, LPARAM const );

	NNetSimuWindow          * m_pAppWindow;
	NNetWorkThreadInterface * m_pNNetWorkThreadInterface;
	WinManager              * m_pWinManager;
	Delay                   * m_pDelay;
	StatusBar               * m_pStatusBar;
	HCURSOR                   m_hCrsrWait;
};				          
