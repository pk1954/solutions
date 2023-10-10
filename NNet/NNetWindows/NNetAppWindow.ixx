// NNetAppWindow.ixx
//
// NNetSimu

module;

#include <string>
#include <fstream>
#include <iostream>
#include <Windows.h>

export module NNetWin32:NNetAppWindow;

import AboutBox;
import Observable;
import SlowMotionRatio;
import DisplayFunctor;
import Win32_Event;
import Win32_Sound;
import ActionTimer;
import WinManager;
import ScriptFile;
import MessagePump;
import BaseWindow;
import ScriptStack;
import Script;
import NNetModelIO;
import NNetModel;
import NNetSignals;
import NNetCommands;
import NNetPreferences;
import :AppTitle;
import :NNetController;
import :StatusBar;
import :SlowMotionControl;
import :DescriptionWindow;
import :CrsrWindow;
import :ScriptHook;
import :UndoRedoMenu;
import :SlowMotionDisplay;
import :ComputeThread;
import :SimulationControl;
import :PerformanceWindow;
import :ParameterDialog;
import :MiniWindow;
import :NNetAppMenu;
import :MainWindow;
import :TimeDisplay;
import :NNetSimuRunning;

using std::wstring;
using std::wofstream;
using std::unique_ptr;

class StatusBarDisplayFunctor : public DisplayFunctor
{
public:
	void Initialize(StatusBar* const pStatusBar, int const iPart)
	{
		m_pStatusBar = pStatusBar;
		m_iPart = iPart;
	}

	void operator() (wstring const& line) final
	{
		if (m_pStatusBar)
		{
			m_wstring = line;
			m_pStatusBar->DisplayInPart(m_iPart, m_wstring);
		}
	}

private:
	wstring     m_wstring{ };
	StatusBar * m_pStatusBar{ nullptr };
	int         m_iPart{ 0 };
};

export class NNetAppWindow : public BaseWindow
{
public:
	NNetAppWindow(wstring const &);
	~NNetAppWindow() override;

	virtual void Start(MessagePump&);
	virtual void Stop();

	NNetAppWindow(NNetAppWindow const&) = delete;  // noncopyable class 
	NNetAppWindow& operator= (NNetAppWindow const&) = delete;  // noncopyable class 

private:

	bool OnCommand(WPARAM const, LPARAM const, PixelPoint const) final;
	void OnNotify (WPARAM const, LPARAM const)                   final;
	void OnChar   (WPARAM const, LPARAM const)                   final;
	bool OnSize   (PIXEL  const, PIXEL  const)                   final;
	void OnClose  ()                                             final;

	void    configureStatusBar();
	void    adjustChildWindows();
	void    processScript() const;
	void    setModelInterface();
	void    replaceModel();
	void    newModel();
	bool    SaveModelAs();
	bool    SaveModel();
	bool    AskAndSave();
	bool    AskNotUndoable();
	void    WriteModel();
	wstring askModelFile(enum class tFileMode const) const;

	bool m_bStarted { false }; // if true, model is visible, all functions available

	int                        m_statusMessagePart{ };
	unique_ptr<Model>          m_upModel;
	NNetModelReaderInterface * m_pNMRI       { nullptr };
	HWND                       m_hwndConsole { nullptr };
	HWND                       m_hwndApp     { nullptr };

	AboutBox                 m_aboutBox;
	ActionTimer              m_atComputation;
	ActionTimer              m_atDisplay;
	AppTitle                 m_appTitle;
	NNetCommandStack         m_cmdStack;
	ComputeThread            m_computeThread;
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
	Observable               m_highlightSigObservable;
	Observable               m_cursorPosObservable;
	Observable               m_staticModelObservable;
	Observable               m_dynamicModelObservable;
	Observable               m_runObservable;
	Observable               m_performanceObservable;
	Observable               m_coordObservable;
	Observable               m_activeSigGenObservable;
	Observable               m_lockModelObservable;
	ParameterDialog          m_parameterDlg;
	PerformanceWindow        m_performanceWindow;
	ScriptFile               m_scriptFile;
	ScriptHook               m_ScriptHook;
	SignalDesigner           m_signalDesigner;
	SimulationControl        m_simulationControl;
	SlowMotionDisplay        m_slowMotionDisplay;
	SlowMotionRatio          m_SlowMotionRatio;
	StatusBar                m_statusBar;
	StatusBarDisplayFunctor  m_statusBarDispFunctor;
	TimeDisplay              m_timeDisplay;
	UndoRedoMenu             m_undoRedoMenu;
	WinManager               m_winManager;
	WinSound                 m_sound;

	bool UserProc(UINT const, WPARAM const, LPARAM const) override;
};
