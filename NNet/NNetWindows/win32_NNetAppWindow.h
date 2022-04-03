// win32_NNetAppWindow.h : 
//
// NNetSimu

#pragma once

#include <fstream>
#include <iostream>
#include "SCRIPT.H"
#include "ScriptStack.h"
#include "NNetModel.h"
#include "Preferences.h"
#include "SlowMotionRatio.h"
#include "ComputeThread.h"
#include "NNetModelImporter.h"
#include "NNetModelExporter.h"
#include "CommandStack.h"
#include "DisplayFunctor.h"
#include "TimeDisplay.h"
#include "SlowMotionDisplay.h"
#include "UndoRedoMenu.h"
#include "MonitorData.h"
#include "win32_ActionTimer.h"
#include "win32_event.h"
#include "win32_sound.h"
#include "win32_appTitle.h"
#include "win32_winManager.h"
#include "win32_scriptHook.h"
#include "win32_MiniWindow.h"
#include "win32_MainWindow.h"
#include "win32_monitorWindow.h"
#include "win32_NNetAppMenu.h"
#include "win32_NNetController.h"
#include "win32_parameterDlg.h"
#include "win32_crsrWindow.h"
#include "win32_descriptionWindow.h"
#include "win32_performanceWindow.h"
#include "win32_simulationControl.h"
#include "win32_slowMotionControl.h"
#include "win32_script.h"
#include "SignalDesigner.h"
#include "NNetModelReaderInterface.h"
#include "NNetModelWriterInterface.h"
#include "NNetModelCommands.h"
#include "NNetColors.h"

class ReadModelResult;
class MessagePump;
class Observable;
class NNetModel;

using std::wofstream;
using std::unique_ptr;

class StatusBarDisplayFunctor : public DisplayFunctor
{
public:
	void Initialize(StatusBar * const pStatusBar, int const iPart)
	{
		m_pStatusBar = pStatusBar;
		m_iPart      = iPart;
	}

	void operator() (wstring const & line) final
	{
		if (m_pStatusBar)
		{
			m_wstring = line;
			m_pStatusBar->DisplayInPart(m_iPart, m_wstring);
		}
	}

private:
	wstring     m_wstring    { };
	StatusBar * m_pStatusBar { nullptr };
	int         m_iPart      { 0 };
};

class NNetAppWindow : public BaseWindow
{
public:
	NNetAppWindow();
	~NNetAppWindow() override;

	virtual void Start(MessagePump &);
	virtual void Stop();

	NNetAppWindow             (NNetAppWindow const &) = delete;  // noncopyable class 
	NNetAppWindow & operator= (NNetAppWindow const &) = delete;  // noncopyable class 

	void StartScript(wstring const &) const;

private:

	bool OnCommand(WPARAM const, LPARAM const, PixelPoint const) final;
	bool OnSize   (PIXEL  const, PIXEL  const) final;
	void OnClose() final;
	void OnPaint() final;

	void configureStatusBar();
	void adjustChildWindows();
	void processScript() const;
	void setModelInterface();
	void replaceModel();
	void newModel();

	bool SaveModelAs();
	bool SaveModel();
	bool AskAndSave();

	void writeModel()
	{
		m_modelExporter.WriteModel(*m_pNMRI);
		m_appTitle.SetUnsavedChanges(false);
	}

	wstring AskModelFile(enum class tFileMode const mode) const
	{
		return ScriptFile::AskForFileName(L"mod", L"Model files", mode);
	}

	bool m_bStarted { false }; // if true, model is visible, all functions available

	int                        m_statusMessagePart { };
	unique_ptr<NNetModel>      m_upModel;
	NNetModelReaderInterface * m_pNMRI       { nullptr };
	SignalGenerator          * m_pSigGenBase { nullptr };
	HWND                       m_hwndConsole { nullptr };
	HWND                       m_hwndApp     { nullptr };

	ActionTimer              m_atComputation          { };
	ActionTimer              m_atDisplay              { };
	AppTitle                 m_appTitle               { };
	CommandStack             m_cmdStack               { };
	ComputeThread            m_computeThread          { };
	CrsrWindow               m_crsrWindow             { };
	DescriptionWindow        m_descWindow             { };
	MainWindow               m_mainNNetWindow         { };
	MiniWindow               m_miniNNetWindow         { };
	MonitorWindow            m_monitorWindow          { };
	NNetAppMenu              m_appMenu                { };
	NNetColors               m_NNetColors             { };
	NNetController           m_NNetController         { };
	NNetModelCommands        m_modelCommands          { };
	NNetModelExporter        m_modelExporter          { };
	NNetModelImporter        m_modelImporter          { };
	NNetModelWriterInterface m_nmwi                   { };
	Observable               m_soundOnObservable      { };
	Observable               m_highlightSigObservable { };
	Observable               m_cursorPosObservable    { };
	Observable               m_staticModelObservable  { };
	Observable               m_dynamicModelObservable { };
	Observable               m_runObservable          { };
	Observable               m_performanceObservable  { };
	Observable               m_coordObservable        { };
	ParameterDialog          m_parameterDlg           { };
	PerformanceWindow        m_performanceWindow      { };
	Preferences              m_preferences            { };
	ScriptFile               m_scriptFile             { };
	ScriptHook               m_ScriptHook             { };
	SignalDesigner           m_signalDesigner         { };
	SimulationControl        m_simulationControl      { };
	SlowMotionDisplay        m_slowMotionDisplay      { };
	SlowMotionRatio          m_SlowMotionRatio        { };
	StatusBar                m_statusBar              { };
	StatusBarDisplayFunctor  m_statusBarDispFunctor   { };
	TimeDisplay              m_timeDisplay            { };
	UndoRedoMenu             m_undoRedoMenu           { };
	WinManager               m_WinManager             { };
	WinSound                 m_sound                  { };

	bool UserProc(UINT const, WPARAM const, LPARAM const) override;
};
