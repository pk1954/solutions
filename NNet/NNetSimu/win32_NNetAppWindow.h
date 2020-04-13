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
#include "NNetReadBuffer.h"
#include "win32_winManager.h"
#include "win32_scriptHook.h"
#include "SlowMotionRatio.h"

// application

#include "win32_NNetWorkThreadInterface.h"
#include "win32_NNetWindow.h"
#include "win32_NNetAppMenu.h"
#include "win32_NNetController.h"
#include "NNetColors.h"

class Param;
class Observable;
class NNetModel;
class CrsrWindow;
class TimeDisplay;
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

	virtual void ProcessCloseMessage( );
	virtual bool ProcessAppCommand( WPARAM const, LPARAM const = 0 );

	void configureStatusBar( );

	bool m_bStarted { false };          // if true, model is visible, all functions available

	Util::Event             m_eventPOI { };

	NNetWorkThreadInterface m_NNetWorkThreadInterface { };
	ActionTimer             m_atComputation           { };
	ActionTimer             m_atDisplay               { };
	ScriptHook              m_ScriptHook              { };
	SlowMotionRatio         m_SlowMotionRatio         { };

	Param                   * m_pParameters              { nullptr };
	SimulationControl       * m_pSimulationControl       { nullptr };
	NNetColors              * m_pNNetColors              { nullptr };
	NNetReadBuffer          * m_pNNetReadBuffer          { nullptr };
	NNetController          * m_pNNetController          { nullptr };
	NNetModel               * m_pModelDataWork           { nullptr };
	NNetWindow              * m_pMainNNetWindow          { nullptr };
	TimeDisplay             * m_pTimeDisplay             { nullptr };
	SlowMotionDisplay       * m_pSlowMotionDisplay       { nullptr };
	CrsrWindow              * m_pCrsrWindow              { nullptr };
	PerformanceWindow       * m_pPerformanceWindow       { nullptr };
	NNetModelStorage        * m_pNNetModelStorage        { nullptr };
	ParameterDialog         * m_pParameterDlg            { nullptr };
	Observable              * m_pCursorPos               { nullptr };
	StatusBarDisplayFunctor * m_pStatusBarDisplayFunctor { nullptr };
};
