// win32_NNetController.h
//
// NNetSimu

#pragma once

class NNetModelWriterInterface;
class SlowMotionRatio;
class NNetModelStorage;
class DisplayFunctor;
class ComputeThread;
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
		NNetModelStorage         * const,
		NNetWindow               * const,
		WinManager               * const,
		NNetModelWriterInterface * const,
		ComputeThread            * const,
		SlowMotionRatio          * const
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

	HCURSOR                    m_hCrsrWait;
	NNetModelStorage         * m_pStorage;
	NNetWindow               * m_pNNetWindow;
	WinManager               * m_pWinManager;
	ComputeThread            * m_pComputeThread;
	NNetModelWriterInterface * m_pModel;
	SlowMotionRatio          * m_pSlowMotionRatio;
	DisplayFunctor           * m_StatusBarDisplay { nullptr };
};				          
