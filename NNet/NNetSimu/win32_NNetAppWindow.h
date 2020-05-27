// win32_NNetAppWindow.h : 
//
// NNetSimu

#pragma once

#include <fstream>
#include <iostream>
#include "win32_event.h"
#include "win32_actionTimer.h"

// infrastructure

#include "win32_baseAppWindow.h"
#include "win32_winManager.h"
#include "win32_scriptHook.h"
#include "SlowMotionRatio.h"

// application

#include "NNetModelWriterInterface.h"
#include "win32_MiniWindow.h"
#include "win32_MainWindow.h"
#include "win32_NNetAppMenu.h"
#include "win32_NNetController.h"
#include "NNetColors.h"

class Param;
class Observable;
class CrsrWindow;
class TimeDisplay;
class NNetModel;
class DrawModel;
class ComputeThread;
class NNetModelReaderInterface;
class SlowMotionDisplay;
class SimulationControl;
class NNetModelStorage;
class ParameterDialog;
class PerformanceWindow;
class StatusBarDisplayFunctor;

class NNetAppWindow : public BaseAppWindow
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

	bool m_bStarted { false };          // if true, model is visible, all functions available

	Util::Event m_eventPOI { };

	wofstream m_traceStream { };

	NNetModelWriterInterface m_NNetWorkThreadInterface { };
	ActionTimer              m_atComputation           { };
	ActionTimer              m_atDisplay               { };
	ScriptHook               m_ScriptHook              { };
	SlowMotionRatio          m_SlowMotionRatio         { };
	Observable               m_cursorPosObservable     { };
	Observable               m_blinkObservable         { };
	Observable               m_modelTimeObservable     { };
	Observable               m_staticModelObservable   { };
	Observable               m_dynamicModelObservable  { };

	Param                    * m_pParameters              { nullptr };
	SimulationControl        * m_pSimulationControl       { nullptr };
	ComputeThread            * m_pComputeThread           { nullptr };
	NNetColors               * m_pNNetColors              { nullptr };
	NNetController           * m_pNNetController          { nullptr };
	NNetModelReaderInterface * m_pModelReaderInterface    { nullptr };
	NNetModel                * m_pModel                   { nullptr };
	MainWindow               * m_pMainNNetWindow          { nullptr };
	MiniWindow               * m_pMiniNNetWindow          { nullptr };
	DrawModel                * m_pDrawModel               { nullptr };
	TimeDisplay              * m_pTimeDisplay             { nullptr };
	SlowMotionDisplay        * m_pSlowMotionDisplay       { nullptr };
	CrsrWindow               * m_pCrsrWindow              { nullptr };
	PerformanceWindow        * m_pPerformanceWindow       { nullptr };
	NNetModelStorage         * m_pNNetModelStorage        { nullptr };
	ParameterDialog          * m_pParameterDlg            { nullptr };
	StatusBarDisplayFunctor  * m_pStatusBarDisplayFunctor { nullptr };
};
