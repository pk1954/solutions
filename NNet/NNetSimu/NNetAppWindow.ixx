// NNetAppWindow.ixx
//
// NNetSimu

export module NNetAppWindow;

import std;
import AboutBox;
import Observable;
import Commands;
import SlowMotionRatio;
import ColLutDesigner;
import DisplayFunctor;
import Win32_Sound;
import WinBasics;
import HiResTimer;
import WinManager;
import ScriptFile;
import MessagePump;
import BaseWindow;
import RunTime;
import StatusBar;
import NNetModelIO;
import NNetModel;
import NNetSignals;
import NNetCommands;
import NNetPreferences;
import ScriptHook;
import SlowMotionControl;
import SlowMotionDisplay;
import StatusBarDisplayFunctor;
import TimeDisplay;
import NNetWin32;
import NNetViewerWindow;

using std::wstring;
using std::wofstream;
using std::unique_ptr;

export class NNetAppWindow : public BaseWindow
{
public:
	NNetAppWindow(wstring const&, MessagePump&);

	void DoGameStuff() { m_compute.DoGameStuff(); }

	NNetAppWindow            (NNetAppWindow const&) = delete;  // noncopyable class 
	NNetAppWindow& operator= (NNetAppWindow const&) = delete;  // noncopyable class 

private:

	bool OnCommand(WPARAM const, LPARAM const, PixelPoint const) final;
	void OnChar   (WPARAM const, LPARAM const)                   final;
	bool OnSize   (PIXEL  const, PIXEL  const)                   final;
	void OnClose  ()                                             final;

	void configureStatusBar();
	void processScript() const;
	void setModelInterface();
	void replaceModel();
	void newModel();
	bool AskNotUndoable();
	void importModel();
	void openModel();

	int                             m_statusMessagePart{ };
	unique_ptr<Model>               m_upModel;
	NNetModelReaderInterface const* m_pNMRI            { nullptr };
	HWND                            m_hwndApp          { nullptr };
	wstring                 const * m_pwstrProductName { nullptr };

	AboutBox                 m_aboutBox;
	Compute                  m_compute;
	HiResTimer               m_atComputation;
	HiResTimer               m_atDisplay;
	AppTitle                 m_appTitle;
	CommandStack             m_cmdStack;
	ColLutDesigner           m_colLutWindow;
	CrsrWindow               m_crsrWindow;
	DescriptionWindow        m_descWindow;
	MainWindow               m_mainNNetWindow;
	MiniWindow               m_miniNNetWindow;
	MonitorWindow            m_monitorWindow;
	NNetAppMenu              m_appMenu;
	NNetColors               m_NNetColors;
	NNetController           m_NNetController;
	NNetModelIO              m_modelIO;
	NNetModelWriterInterface m_nmwi;
	NNetSimuRunning          m_simuRunning;
	NNetViewerWindow         m_viewerWindow;
	Observable               m_highlightSigObservable;
	Observable               m_cursorPosObservable;
	Observable               m_staticModelObservable;
	Observable               m_dynamicModelObservable;
	Observable               m_runObservable;
	Observable               m_performanceObservable;
	Observable               m_coordObservable;
	Observable               m_activeSigGenObservable;
	ParameterDialog          m_parameterDlg;
	PerformanceWindow        m_performanceWindow;
	ScriptFile               m_scriptFile;
	ScriptHook               m_ScriptHook;
	SignalDesigner           m_signalDesigner;
	SimulationControl        m_simulationControl;
	SlowMotionDisplay        m_slowMotionDisplay;
	SlowMotionRatio          m_slowMotionRatio;
	StatusBar                m_statusBar;
	StatusBarDisplayFunctor  m_statusBarDispFunctor;
	TimeDisplay              m_timeDisplay;
	UndoRedoMenu             m_undoRedoMenu;
	WinSound                 m_sound;

	bool UserProc(UINT const, WPARAM const, LPARAM const) override;
};
