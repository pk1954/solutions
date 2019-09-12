// win32_NNetSimuController.h
//

#pragma once

#include "win32_controller.h"

class NNetSimuWindow;
class NNetWorkThreadInterface;
class Delay;
class StatusBar;
class WinManager;
class NNetSimuMenu;

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
		Delay                   * const,
		NNetSimuMenu            * const
	);

	void ProcessCommand( WPARAM const, LPARAM const = 0 );

private:
	bool processUIcommand( int const, LPARAM const );

	NNetSimuWindow          * m_pAppWindow;
	NNetWorkThreadInterface * m_pNNetWorkThreadInterface;
	WinManager              * m_pWinManager;
	Delay                   * m_pDelay;
	StatusBar               * m_pStatusBar;
	NNetSimuMenu            * m_pAppMenu;
	HCURSOR                   m_hCrsrWait;
};				          
