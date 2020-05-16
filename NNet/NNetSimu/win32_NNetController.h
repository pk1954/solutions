// win32_NNetController.h
//
// NNetSimu

#pragma once

class WorkThreadInterface;
class SlowMotionRatio;
class NNetModelStorage;
class DisplayFunctor;
class NNetAppMenu;
class NNetModel;
class NNetWindow;
class WinManager;
class Param;

class NNetController
{
public:
	NNetController
	( 
		NNetModelStorage    * const,
		NNetWindow          * const,
		WinManager          * const,
		WorkThreadInterface * const,
		SlowMotionRatio     * const
	);

	virtual ~NNetController( );

	void SetStatusBarDisplay( DisplayFunctor * const func )
	{
		m_StatusBarDisplay = func;
	}

	bool HandleCommand( WPARAM const, LPARAM const, MicroMeterPoint const );

private:

	bool processUIcommand   ( int const, LPARAM const );
	bool processModelCommand( int const, LPARAM const, MicroMeterPoint const );

	HCURSOR               m_hCrsrWait;
	NNetModelStorage    * m_pStorage;
	NNetWindow          * m_pNNetWindow;
	WinManager          * m_pWinManager;
	WorkThreadInterface * m_pWorkThreadInterface;
	SlowMotionRatio     * m_pSlowMotionRatio;
	DisplayFunctor      * m_StatusBarDisplay { nullptr };
};				          
