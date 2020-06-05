// win32_NNetAppWindow.h : 
//
// NNetSimu

#pragma once

#include <fstream>
#include <iostream>
#include "SCRIPT.H"
#include "SlowMotionRatio.h"
#include "ComputeThread.h"
#include "NNetModelStorage.h"
#include "DrawModel.h"
#include "DisplayFunctor.h"
#include "TimeDisplay.h"
#include "SlowMotionDisplay.h"
#include "win32_event.h"
#include "win32_actionTimer.h"
#include "win32_winManager.h"
#include "win32_scriptHook.h"
#include "win32_MiniWindow.h"
#include "win32_MainWindow.h"
#include "win32_NNetAppMenu.h"
#include "win32_NNetController.h"
#include "win32_parameterDlg.h"
#include "win32_crsrWindow.h"
#include "win32_performanceWindow.h"
#include "win32_simulationControl.h"
#include "win32_slowMotionControl.h"
#include "NNetModelReaderInterface.h"
#include "NNetModelWriterInterface.h"
#include "NNetColors.h"

class Observable;
class NNetModel;

using std::wofstream;

class StatusBarDisplayFunctor : public DisplayFunctor
{
public:
	void Initialize( StatusBar * const pStatusBar, int const iPart )
	{
		m_pStatusBar = pStatusBar;
		m_iPart      = iPart;
	}

	virtual void operator() ( wstring const & line )
	{
		if ( m_pStatusBar )
		{
			m_wstring = line;
			m_pStatusBar->DisplayInPart( m_iPart, m_wstring );
		}
	}

private:
	wstring     m_wstring     { };
	StatusBar * m_pStatusBar { nullptr };
	int         m_iPart      { 0 };
};

class NNetAppWindow : public BaseWindow
{
public:
	NNetAppWindow( );
	~NNetAppWindow( );

	virtual void Start( );
	virtual void Stop( );

private:
	NNetAppWindow             ( NNetAppWindow const & );  // noncopyable class 
	NNetAppWindow & operator= ( NNetAppWindow const & );  // noncopyable class 

	virtual bool OnCommand( WPARAM const, LPARAM const, PixelPoint const );
	virtual void OnClose( );

	void configureStatusBar( );
	void adjustChildWindows( );

	bool m_bStarted { false }; // if true, model is visible, all functions available

	HWND                     m_hwndConsole            { nullptr };
	HWND                     m_hwndApp                { nullptr };
	int                      m_statusMessagePart      { };
	wofstream                m_traceStream            { };
	WinManager               m_WinManager             { };
	StatusBar                m_StatusBar              { };
	Script                   m_script                 { };
	NNetAppMenu              m_appMenu                { };
	ActionTimer              m_atComputation          { };
	ActionTimer              m_atDisplay              { };
	ScriptHook               m_ScriptHook             { };
	SlowMotionRatio          m_SlowMotionRatio        { };
	Observable               m_cursorPosObservable    { };
	Observable               m_blinkObservable        { };
	Observable               m_modelTimeObservable    { };
	Observable               m_staticModelObservable  { };
	Observable               m_dynamicModelObservable { };
	NNetModelReaderInterface m_modelReaderInterface   { };
	NNetModelWriterInterface m_modelWriterInterface   { };
	ComputeThread            m_computeThread          { };
	CrsrWindow               m_crsrWindow             { };
	PerformanceWindow        m_performanceWindow      { };
	MainWindow               m_mainNNetWindow         { };
	MiniWindow               m_miniNNetWindow         { };
	ParameterDialog          m_parameterDlg           { };
	Param                    m_parameters             { };
	NNetModelStorage         m_modelStorage           { };
	StatusBarDisplayFunctor  m_statusBarDispFunctor   { };
	NNetModel                m_model                  { };
	DrawModel                m_drawModel              { };
	NNetColors               m_NNetColors             { };
	NNetController           m_NNetController         { };
	SimulationControl        m_simulationControl      { };
	TimeDisplay              m_timeDisplay            { };
	SlowMotionDisplay        m_slowMotionDisplay      { };

	virtual LRESULT UserProc( UINT const, WPARAM const, LPARAM const );
};
