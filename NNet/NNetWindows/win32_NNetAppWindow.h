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
	void openSignalDesigner();
	void processScript() const;
	void setModelInterface();

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

	unique_ptr<NNetModel>      m_upModel;
	NNetModelReaderInterface * m_pNMRI { nullptr };

	NNetModelWriterInterface m_nmwi                   { };
	HWND                     m_hwndConsole            { nullptr };
	HWND                     m_hwndApp                { nullptr };
	WinSound                 m_sound                  { };
	int                      m_statusMessagePart      { };
	WinManager               m_WinManager             { };
	StatusBar                m_StatusBar              { };
	AppTitle                 m_appTitle               { };
	NNetAppMenu              m_appMenu                { };
	ActionTimer              m_atComputation          { };
	ActionTimer              m_atDisplay              { };
	ScriptHook               m_ScriptHook             { };
	SlowMotionRatio          m_SlowMotionRatio        { };
	Observable               m_soundOnObservable      { };
	Observable               m_highlightSigObservable { };
	Observable               m_cursorPosObservable    { };
	Observable               m_staticModelObservable  { };
	Observable               m_dynamicModelObservable { };
	Observable               m_runObservable          { };
	Observable               m_performanceObservable  { };
	Observable               m_coordObservable        { };
	NNetModelCommands        m_modelCommands          { };
	ComputeThread            m_computeThread          { };
	DescriptionWindow        m_descWindow             { };
	CrsrWindow               m_crsrWindow             { };
	PerformanceWindow        m_performanceWindow      { };
	MainWindow               m_mainNNetWindow         { };
	MiniWindow               m_miniNNetWindow         { };
	MonitorWindow            m_monitorWindow          { };
	ParameterDialog          m_parameterDlg           { };
	NNetModelImporter        m_modelImporter          { };
	NNetModelExporter        m_modelExporter          { };
	StatusBarDisplayFunctor  m_statusBarDispFunctor   { };
	NNetColors               m_NNetColors             { };
	NNetController           m_NNetController         { };
	SimulationControl        m_simulationControl      { };
	TimeDisplay              m_timeDisplay            { };
	SlowMotionDisplay        m_slowMotionDisplay      { };
	Preferences              m_preferences            { };
	CommandStack             m_cmdStack               { };
	UndoRedoMenu             m_undoRedoMenu           { };
	ScriptFile               m_scriptFile             { };

	bool UserProc(UINT const, WPARAM const, LPARAM const) override;
};
