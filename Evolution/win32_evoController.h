// win32_evoController.h
//

#pragma once

#include <fstream>

class AppWindow;
class Delay;
class StatusBar;
class EvoWorkThreadInterface;
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

	void EvoController::Initialize
	( 
		AppWindow              * const,
		std::wostream          *,
		EvoWorkThreadInterface * const,
		WinManager             * const,
		EvoHistorySysGlue      * const,
		Delay                  * const,
		ColorManager           * const,
		AppMenu                * const,
		StatusBar              * const,
		GridWindow             * const,
		EditorWindow           * const
	);

	void ProcessCommand( WPARAM const, LPARAM const = 0 );

private:
	void scriptDialog( );
	bool processUIcommand( int const, LPARAM const );

    std::wostream          * m_pTraceStream;
	AppWindow              * m_pAppWindow;
	EvoWorkThreadInterface * m_pWorkThreadInterface;
	WinManager             * m_pWinManager;
	EvoHistorySysGlue      * m_pEvoHistGlue;
	Delay                  * m_pDelay;
	StatusBar              * m_pStatusBar;
	GridWindow             * m_pGridWindow;
	EditorWindow           * m_pEditorWindow;
	ColorManager           * m_pColorManager;
	AppMenu                * m_pAppMenu;
	HCURSOR                  m_hCrsrWait;
};
