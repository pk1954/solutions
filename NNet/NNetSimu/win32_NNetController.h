// win32_NNetController.h
//
// NNetSimu

#pragma once

class NNetWorkThreadInterface;
class SlowMotionRatio;
class NNetModelStorage;
class NNetAppMenu;
class NNetModel;
class NNetWindow;
class WinManager;
class StatusBar;

class NNetController
{
public:
	NNetController
	( 
		NNetModel               * const,
		NNetModelStorage        * const,
		NNetWindow              * const,
		WinManager              * const,
		StatusBar               * const,
		NNetWorkThreadInterface * const,
		SlowMotionRatio         * const
	);

	virtual ~NNetController( );

	bool ProcessUIcommand   ( int const, LPARAM const );
	bool ProcessModelCommand( int const, LPARAM const );

private:

	NNetModel               * m_pNNetModel;
	NNetModelStorage        * m_pStorage;
	NNetWindow              * m_pNNetWindow;
	WinManager              * m_pWinManager;
	StatusBar               * m_pStatusBar;
	NNetWorkThreadInterface * m_pNNetWorkThreadInterface;
	SlowMotionRatio         * m_pSlowMotionRatio;
	HCURSOR                   m_hCrsrWait;
	bool                      m_bUnsavedChanges;
};				          
