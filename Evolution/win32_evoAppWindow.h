// win32_evoAppWindow.h : 
//
// Evolution

#pragma once

#include <fstream>
#include <iostream>
#include "win32_event.h"
#include "win32_delay.h"
#include "win32_actionTimer.h"

// infrastructure

#include "win32_appWindowInterface.h"
#include "win32_protocolServer.h"
#include "win32_scriptHook.h"
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
class EvoEditorWindow;
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

class EvoAppWindow : public AppWindowInterface
{
public:
    EvoAppWindow( );
	~EvoAppWindow( );

	virtual void Start( );
	virtual void Stop( );

private:
	EvoAppWindow             ( EvoAppWindow const & );  // noncopyable class 
    EvoAppWindow & operator= ( EvoAppWindow const & );  // noncopyable class 

	virtual void ProcessAppCommand( WPARAM const, LPARAM const = 0 );

	void configureStatusBar( );

	Util::Event              m_eventPOI;
					         
	ActionTimer              m_atComputation;
	ActionTimer              m_atDisplay;
	Delay                    m_Delay;
	EvoHistorySysGlue        m_EvoHistGlue;
	ColorManager             m_ColorManager;
	D3D_driver               m_D3d_driver;
    ScriptHook               m_ScriptHook;
	ProtocolServer           m_protocolServer;
					       
	EvoWorkThreadInterface * m_pEvoWorkThreadInterface;
	EvoReadBuffer          * m_pEvoReadBuffer;
	EvoController          * m_pEvoController;
	EvolutionCore          * m_pEvoCore4Display;
	GridWindow             * m_pMainGridWindow;
	EvoModelDataGlue       * m_pModelDataWork;
					     
    GridWindow             * m_pMiniGridWindow;
    EvoEditorWindow        * m_pEvoEditorWindow;
	DspOptWindow           * m_pDspOptWindow;
	PerformanceWindow      * m_pPerfWindow;
	CrsrWindow             * m_pCrsrWindow;
	StatisticsWindow       * m_pStatistics;
	GraphicsInterface      * m_pGraphics;
	FocusPoint             * m_pFocusPoint;
	GenerationDisplay      * m_pGenerationDisplay;
};
