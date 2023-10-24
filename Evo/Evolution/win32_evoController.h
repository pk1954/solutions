// win32_evoController.h
//
// Evolution

module;

class EvoAppWindow;
class Delay;
class StatusBar;
class EvoWorkThreadInterface;
class EvoHistorySysGlue;
class WinManager;
class GridWindow;
class EvoEditorWindow;
class ColorManager;

class EvoController 
{
public:
	EvoController
	(
		WinManager        * const,
		EvoHistorySysGlue * const,
		Delay             * const,
		ColorManager      * const,
		StatusBar         * const,
		GridWindow        * const,
		EvoEditorWindow   * const
	);

	virtual ~EvoController();

	void EvoController::Initialize
	(
		EvoAppWindow           * const,
		EvoWorkThreadInterface * const
	);

	bool ProcessUIcommand   (int const, LPARAM const);
	bool ProcessModelCommand(int const, LPARAM const);

private:
	void setSizeTrackBar(PIXEL const);

	EvoAppWindow           * m_pAppWindow;
	EvoWorkThreadInterface * m_pEvoWorkThreadInterface;
	WinManager             * m_pWinManager;
	EvoHistorySysGlue      * m_pEvoHistGlue;
	Delay                  * m_pDelay;
	StatusBar              * m_pStatusBar;
	GridWindow             * m_pGridWindow;
	EvoEditorWindow        * m_pEvoEditorWindow;
	ColorManager           * m_pColorManager;
	HCURSOR                  m_hCrsrWait;
};
