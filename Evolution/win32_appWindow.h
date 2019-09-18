// win32_appWindow.h : 
//

#pragma once

#include <fstream>
#include <iostream>
#include "win32_event.h"
#include "win32_delay.h"
#include "win32_actionTimer.h"

// infrastructure

#include "win32_baseAppWindow.h"
#include "win32_protocolServer.h"
#include "win32_scriptHook.h"
#include "win32_readBuffer.h"
#include "win32_colorManager.h"
#include "win32_EvoWorkThreadInterface.h"
#include "EvoHistorySysGlue.h"
#include "EvoReadBuffer.h"
#include "GDI_driver.h"
#include "D3D_driver.h"

// application

#include "win32_evoAppMenu.h"
#include "win32_evoController.h"

class Script;
class FocusPoint;
class GridWindow;
class EditorWindow;
class StatisticsWindow;
class CrsrWindow;
class PerformanceWindow;
class GenerationDisplay;
class StatusBar;
class ScriptHook;
class GraphicsInterface;
class D3D_driver;
class EvoModelDataGlue;
class EvolutionCore;
class DspOptWindow;
class EvoController;
class AppMenu;

class AppWindow : public BaseAppWindow
{
public:
    AppWindow( );
	void Start( );
	void Stop( );
	~AppWindow( );

private:
	AppWindow             ( AppWindow const & );  // noncopyable class 
    AppWindow & operator= ( AppWindow const & );  // noncopyable class 

	void configureStatusBar( );

	HWND                   m_hwndApp;
	Util::Event            m_event;
					       
	ActionTimer            m_atComputation;
	ActionTimer            m_atDisplay;
	Delay                  m_Delay;
	ColorManager           m_ColorManager;
	D3D_driver             m_D3d_driver;
	EvoReadBuffer          m_EvoReadBuffer;
	EvoWorkThreadInterface m_EvoWorkThreadInterface;
	EvoHistorySysGlue      m_EvoHistGlue;
    ScriptHook             m_ScriptHook;
	ProtocolServer         m_protocolServer;
					       
	EvoController          * m_pEvoController;
	GridWindow             * m_pMainGridWindow;
    GridWindow             * m_pMiniGridWindow;
    EditorWindow           * m_pEditorWindow;
	DspOptWindow           * m_pDspOptWindow;
	PerformanceWindow      * m_pPerfWindow;
	CrsrWindow             * m_pCrsrWindow;
	StatisticsWindow       * m_pStatistics;
	GraphicsInterface      * m_pGraphics;
	EvoModelDataGlue       * m_pModelDataWork;
	EvolutionCore          * m_pEvoCore4Display;
	FocusPoint             * m_pFocusPoint;
	GenerationDisplay      * m_pGenerationDisplay;

    std::wofstream m_traceStream;
};
