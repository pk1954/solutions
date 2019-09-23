// win32_NNetController.h
//
// NNetSimu

#pragma once

class NNetWorkThreadInterface;
class NNetEditorWindow;
class NNetAppWindow;
class NNetAppMenu;
class NNetWindow;
class WinManager;
class StatusBar;
class Delay;

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
		Delay                   * const
	);

	bool ProcessUIcommand   ( int const, LPARAM const );
	bool ProcessModelCommand( int const, LPARAM const );

private:

	NNetAppWindow           * m_pAppWindow;
	NNetWorkThreadInterface * m_pNNetWorkThreadInterface;
	WinManager              * m_pWinManager;
	Delay                   * m_pDelay;
	StatusBar               * m_pStatusBar;
	NNetWindow              * m_pNNetWindow;
	NNetEditorWindow        * m_pNNetEditorWindow;
	HCURSOR                   m_hCrsrWait;
};				          
