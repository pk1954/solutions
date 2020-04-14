// win32_NNetController.h
//
// NNetSimu

#pragma once

class NNetWorkThreadInterface;
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
		NNetModel               * const,
		NNetModelStorage        * const,
		NNetWindow              * const,
		WinManager              * const,
		NNetWorkThreadInterface * const,
		SlowMotionRatio         * const
	);

	virtual ~NNetController( );

	void SetDisplayFunctor( DisplayFunctor * const func )
	{
		m_pDisplayFunctor = func;
	}

	bool ProcessUIcommand   ( int const, LPARAM const );
	bool ProcessModelCommand( int const, LPARAM const );

private:

	HCURSOR                   m_hCrsrWait;
	NNetModel               * m_pNNetModel;
	NNetModelStorage        * m_pStorage;
	NNetWindow              * m_pNNetWindow;
	WinManager              * m_pWinManager;
	NNetWorkThreadInterface * m_pNNetWorkThreadInterface;
	SlowMotionRatio         * m_pSlowMotionRatio;
	DisplayFunctor          * m_pDisplayFunctor { nullptr };
};				          
