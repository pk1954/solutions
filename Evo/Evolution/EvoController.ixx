// EvoController.ixx
//
// Evolution

export module EvoController;

import Types;
import WinBasics;
import EvoAppWindow;
import StatusBar;
import EvoWorkThread;
import EvoHistorySysGlue;
import WinManager;
import GridWindow;
import EvoEditorWindow;
import ColorManager;

export class EvoController 
{
public:
	EvoController
	(
		WinManager        * const,
		EvoHistorySysGlue * const,
		ColorManager      * const,
		StatusBar         * const,
		GridWindow        * const,
		EvoEditorWindow   * const
	);

	virtual ~EvoController();

	void Initialize
	(
		EvoAppWindow  * const,
		EvoWorkThread * const
	);

	bool ProcessUIcommand   (int const, LPARAM const);
	bool ProcessModelCommand(int const, LPARAM const);

private:
	void setSizeTrackBar(PIXEL const);

	EvoAppWindow      * m_pAppWindow;
	EvoWorkThread     * m_pEvoWorkThread;
	WinManager        * m_pWinManager;
	EvoHistorySysGlue * m_pEvoHistGlue;
	StatusBar         * m_pStatusBar;
	GridWindow        * m_pGridWindow;
	EvoEditorWindow   * m_pEvoEditorWindow;
	ColorManager      * m_pColorManager;
	HCURSOR             m_hCrsrWait;
};
