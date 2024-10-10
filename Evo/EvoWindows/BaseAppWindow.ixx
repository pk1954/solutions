// BaseAppWindow.ixx
//
// EvoWindows

export module BaseAppWindow;

import std;
import WinBasics;
import BaseWindow;
import WinManager;
import StatusBar;
//import WorkThreadInterface;
import HistoryLib;
import WinHistLib;
import AppMenu;

using std::wofstream;

export class BaseAppWindow : public BaseWindow
{
public:
	BaseAppWindow();
	virtual ~BaseAppWindow(); 

	void Initialize
	(
		//WorkThreadInterface * const, 
		bool const
	);

	void Start(BaseWindow * const);

	virtual void ProcessAppCommand(WPARAM const, LPARAM const = 0) = 0;

protected:
	HWND       m_hwndApp;
	WinManager m_WinManager;
	StatusBar  m_StatusBar;

	AppMenu       * m_pAppMenu;        // allocated by application
	HistorySystem * m_pHistorySystem;  // allocated here

	virtual void Stop();

    bool ProcessFrameworkCommand(WPARAM const, LPARAM const = 0);

private:

	HWND m_hwndConsole;
	bool m_bStarted;          // if true, model is visible, all functions available
	bool m_bUseHistorySystem;

	//HistWindow     m_HistWindow;
	//HistInfoWindow m_HistInfoWindow;

	BaseWindow          * m_pModelWindow;
	//WorkThreadInterface * m_pWorkThreadInterface;

	wofstream m_traceStream;

	void adjustChildWindows();

	virtual bool UserProc(UINT const, WPARAM const, LPARAM const);
};