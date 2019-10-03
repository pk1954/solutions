// win32_NNetController.h
//
// NNetSimu

#pragma once

class NNetWorkThreadInterface;
class SlowMotionRatio;
class NNetEditorWindow;
class NNetAppWindow;
class NNetAppMenu;
class NNetWindow;
class WinManager;
class StatusBar;

class NNetController
{
public:
	NNetController
	( 
		NNetAppWindow           * const,
		NNetWindow              * const,
		NNetEditorWindow        * const,
		WinManager              * const,
		StatusBar               * const,
		NNetWorkThreadInterface * const,
		SlowMotionRatio         * const
	);

	virtual ~NNetController( );

	bool ProcessUIcommand   ( int const, LPARAM const );
	bool ProcessModelCommand( int const, LPARAM const );

private:
	void setSizeTrackBar( NanoMeter const );

	NNetAppWindow           * m_pAppWindow;
	NNetWindow              * m_pNNetWindow;
	NNetEditorWindow        * m_pNNetEditorWindow;
	WinManager              * m_pWinManager;
	StatusBar               * m_pStatusBar;
	NNetWorkThreadInterface * m_pNNetWorkThreadInterface;
	SlowMotionRatio         * m_pSlowMotionRatio;
	HCURSOR                   m_hCrsrWait;
};				          
