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
		WinManager       * const,
		NNetWindow       * const,
		StatusBar        * const,
		NNetEditorWindow * const
	);

	virtual ~NNetController( );

	void Initialize
	( 
		NNetAppWindow           * const,
		NNetWorkThreadInterface * const,
		SlowMotionRatio         * const
	);

	bool ProcessUIcommand   ( int const, LPARAM const );
	bool ProcessModelCommand( int const, LPARAM const );

private:
	void setSizeTrackBar( NanoMeter const );

	NNetAppWindow           * m_pAppWindow;
	NNetWorkThreadInterface * m_pNNetWorkThreadInterface;
	WinManager              * m_pWinManager;
	StatusBar               * m_pStatusBar;
	NNetWindow              * m_pNNetWindow;
	NNetEditorWindow        * m_pNNetEditorWindow;
	SlowMotionRatio         * m_pSlowMotionRatio;
	HCURSOR                   m_hCrsrWait;
};				          
