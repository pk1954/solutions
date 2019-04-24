// win32_evoController.h
//

#pragma once

#include <fstream>

class PerformanceWindow;
class StatusBar;
class WorkThreadInterface;
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
		PerformanceWindow   * const,
		StatusBar           * const,
		GridWindow          * const,
		EditorWindow        * const,
		ColorManager        * const,
		AppMenu             * const
	);

	void ProcessCommand( WPARAM const, LPARAM const );

private:
	void scriptDialog( );

	BOOL                  m_bTrace;
    std::wostream       * m_pTraceStream;
	WorkThreadInterface * m_pWorkThreadInterface;
	WinManager          * m_pWinManager;
    PerformanceWindow   * m_pPerformanceWindow;
	StatusBar           * m_pStatusBar;
	GridWindow          * m_pGridWindow;
	EditorWindow        * m_pEditorWindow;
	ColorManager        * m_pColorManager;
	AppMenu             * m_pAppMenu;
};
