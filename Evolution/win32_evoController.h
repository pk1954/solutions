// win32_evoController.h
//

#pragma once

#include <fstream>

class Delay;
class StatusBar;
class WorkThreadInterface;
class EvoHistorySysGlue;
class WinManager;
class GridWindow;
class EditorWindow;
class ColorManager;
class AppMenu;

class EvoController
{
public:
	EvoController( );

	virtual ~EvoController( );

	void EvoController::Start
	( 
		std::wostream       *, 
		WorkThreadInterface * const,
		WinManager          * const,
		EvoHistorySysGlue   * const,
		Delay               * const,
		StatusBar           * const,
		GridWindow          * const,
		EditorWindow        * const,
		ColorManager        * const,
		AppMenu             * const
	);

	void ProcessCommand( WPARAM const, LPARAM const = 0 );

private:
	void scriptDialog( );
	bool processUIcommand( int const, LPARAM const );

	BOOL                  m_bTrace;
    std::wostream       * m_pTraceStream;
	WorkThreadInterface * m_pWorkThreadInterface;
	WinManager          * m_pWinManager;
	EvoHistorySysGlue   * m_pEvoHistGlue;
	Delay               * m_pDelay;
	StatusBar           * m_pStatusBar;
	GridWindow          * m_pGridWindow;
	EditorWindow        * m_pEditorWindow;
	ColorManager        * m_pColorManager;
	AppMenu             * m_pAppMenu;
};
