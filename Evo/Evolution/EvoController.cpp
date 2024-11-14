// win32_evoController.cpp
//
// Evolution


module EvoController;

import EvoHistorySysGlue;
import EvoAppWindow;
import SpeedControl;
import ZoomControl;
import StatusBar;
import EvoEditor;
import EvoAppMenu;
import GridWindow;
import PackGridPoint;
import ColorManager;
import ResetDlg;
import EvoWorkThread;
import LogarithmicTrackbar;
import RunTime;
import WinManager;
import EvoCoreLib;
import EvoConfig;
import AboutBox;
import Resource;

using namespace std::chrono;

EvoController::EvoController
(
	WinManager        * const pWinManager,
	EvoHistorySysGlue * const pEvoHistGlue,
	ColorManager      * const pColorManager,
	StatusBar         * const pStatusBar,
	GridWindow        * const pGridWindow,
	EvoEditorWindow   * const pEditorWindow
) :
	m_pAppWindow      (nullptr),
	m_pEvoWorkThread  (nullptr),
	m_pWinManager     (pWinManager  ),
	m_pEvoHistGlue    (pEvoHistGlue ),
	m_pColorManager   (pColorManager),
	m_pStatusBar      (pStatusBar   ),
	m_pGridWindow     (pGridWindow  ),
	m_pEvoEditorWindow(pEditorWindow),
	m_hCrsrWait       (nullptr)
{ }

EvoController::~EvoController()
{
	m_pAppWindow       = nullptr;
	m_pEvoWorkThread   = nullptr;
	m_pWinManager      = nullptr;
	m_pEvoHistGlue     = nullptr;
	m_pColorManager    = nullptr;
    m_pStatusBar       = nullptr;
	m_pGridWindow      = nullptr;
	m_pEvoEditorWindow = nullptr;
	m_hCrsrWait        = nullptr;
}

void EvoController::Initialize
(
 	EvoAppWindow  * const pAppWindow,
	EvoWorkThread * const pEvoWorkThread
)
{
	m_pEvoWorkThread = pEvoWorkThread;
	m_pAppWindow     = pAppWindow;
	m_hCrsrWait     = LoadCursor(NULL, IDC_WAIT);
}

bool EvoController::ProcessUIcommand(int const wmId, LPARAM const lParam)
{
	switch (wmId)
	{
	case IDM_DISP_WINDOW:
	case IDM_EDIT_WINDOW:
	case IDM_MAIN_WINDOW:
	case IDM_STAT_WINDOW:
	case IDM_CRSR_WINDOW:
	case IDM_HIST_INFO:
	case IDM_PERF_WINDOW:
	case IDM_MINI_WINDOW:
	case IDM_HIST_WINDOW:
		SendMessage(m_pWinManager->GetHWND(wmId), WM_COMMAND, IDM_WINDOW_ON, 0);
		break;

	case IDD_TOGGLE_STRIP_MODE:
		m_pGridWindow->ToggleStripMode();
		break;

	case IDD_TOGGLE_CLUT_MODE:
		m_pGridWindow->ToggleClutMode();
		break;

	case IDD_TOGGLE_COORD_DISPLAY:
		EvoConfig::SetConfigValueBoolOp(EvoConfig::tId::showGridPointCoords, tBoolOp::opToggle);
		m_pGridWindow->Notify(TRUE);
		break;

	case IDM_FIT_ZOOM:
		m_pGridWindow->Fit2Rect();
		setSizeTrackBar(m_pGridWindow->GetFieldSize());
		break;
		
	case IDM_MAX_SPEED:
		{
			HWND hwndStatusBar = m_pStatusBar->GetWindowHandle();
			m_pStatusBar->SetTrackBarPos(IDM_SIMULATION_SPEED,  Cast2Long(MAX_DELAY.count()));                
			EnableWindow(GetDlgItem(hwndStatusBar, IDM_MAX_SPEED), FALSE);
		}
		break;

	case IDM_TRACKBAR:
		switch (lParam)
		{
		case IDM_ZOOM_TRACKBAR:
		{
			LONG const lLogicalPos = m_pStatusBar->GetTrackBarPos(IDM_ZOOM_TRACKBAR);
			LONG const lValue      = lLogicalPos;
			LONG const lPos        = LogarithmicTrackbar::TrackBar2ValueL(lValue);
			ProcessUIcommand(IDM_ZOOM_TRACKBAR, lPos);
		}
		break;

		case IDM_SIMULATION_SPEED:
		{
			LONG const lLogicalPos = m_pStatusBar->GetTrackBarPos(IDM_SIMULATION_SPEED);
			LONG const lValue      = LogarithmicTrackbar::Value2TrackbarL(Cast2Long(MAX_DELAY.count())) - lLogicalPos;
			LONG const lPos        = LogarithmicTrackbar::TrackBar2ValueL(lValue);
			EnableWindow(m_pStatusBar->GetDlgItem(IDM_MAX_SPEED), TRUE);
			//m_pDelay->SetDelay(milliseconds(lPos));
		}
		break;

		default:
			Assert(false);
		}
		break;

	case IDD_EDITOR:
		ShowWindow(m_pStatusBar->GetDlgItem(IDM_EDIT_WINDOW), ! m_pEvoEditorWindow->IsWindowVisible());
		break;

	case IDM_ZOOM_OUT:
	case IDM_ZOOM_IN:
		m_pGridWindow->Zoom(wmId == IDM_ZOOM_IN);
		setSizeTrackBar(m_pGridWindow->GetFieldSize());
		break;

	case IDM_SET_ZOOM:
		m_pGridWindow->SetFieldSize(PIXEL(Cast2Short(lParam)));
		setSizeTrackBar(PIXEL(Cast2Short(lParam)));
		break;

	case IDM_ZOOM_TRACKBAR:  // comes from trackbar in statusBar
		(void)m_pGridWindow->SetFieldSize(PIXEL(Cast2Short(lParam)));
		break;

	case IDM_REFRESH:
		m_pGridWindow->Notify(lParam != 0);
		break;

	default:
		return FALSE; // command has not been processed
	}

	return TRUE;  // command has been processed
}

bool EvoController::ProcessModelCommand(int const wmId, LPARAM const lParam)
{
	switch (wmId)
	{
	case IDM_RUN:
		m_pEvoEditorWindow->SendClick(IDM_MOVE);   // change edit mode to move
		SpeedControl::Adjust(TRUE, m_pEvoWorkThread);
		return true;

	case IDM_STOP:
		SpeedControl::Adjust(FALSE, m_pEvoWorkThread);
		return true;

	case IDM_RESET:
	{
		int     iRes    = ResetDialog::Show(m_pAppWindow->GetWindowHandle());
		HCURSOR crsrOld = SetCursor(m_hCrsrWait);
		switch (iRes)
		{
		case IDM_SOFT_RESET:
			m_pEvoWorkThread->PostReset(FALSE);
			break;

		case IDM_HISTORY_RESET:
			m_pEvoWorkThread->PostReset(TRUE);
			break;

		case IDM_HARD_RESET:
			m_pAppWindow->Stop();
			GridDimensions::DefineGridSize
			(
				GridCoord(ResetDialog::GetNewWidth()), 
				GridCoord(ResetDialog::GetNewHeight()), 
				ResetDialog::GetNewNrOfNeighbors()
			);
			m_pAppWindow->Start();
		}
		SetCursor(crsrOld);
	}
	break;

	case IDM_SOFT_RESET:
		m_pEvoWorkThread->PostReset(FALSE);
		break;

	case IDM_HISTORY_RESET:
		m_pEvoWorkThread->PostReset(TRUE);
		break;

	case IDM_GOTO_ORIGIN:
		m_pEvoWorkThread->PostGotoOrigin(UnpackFromLParam(lParam));
		break;

	case IDM_GOTO_DEATH:
		m_pEvoWorkThread->PostGotoDeath(UnpackFromLParam(lParam));
		break;

	case IDM_SET_POI:
		m_pEvoWorkThread->PostSetPOI(UnpackFromLParam(lParam));
		break;

	case IDM_SCRIPT_DIALOG:
		{
			wstring const wstrFile { ScriptDialog() };
			if (! wstrFile.empty())
			{
				std::wcout << L"Processing script file " << wstrFile << L"...";
				RunTime::ProcessScript(wstrFile);
			}
		}
		break;

	case IDM_ESCAPE:
		m_pGridWindow->Escape();
		break;

	default:
		return true;
	}

	return false;
}

void EvoController::setSizeTrackBar(PIXEL const pixFieldSize)
{ 
	m_pStatusBar->SetTrackBarPos(IDM_ZOOM_TRACKBAR, LogarithmicTrackbar::Value2TrackbarL(pixFieldSize.GetValue())); 
}
