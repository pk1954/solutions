// win32_appWindow.h : 
//

#pragma once

#include <fstream>
#include <iostream>
#include "win32_baseWindow.h"
#include "win32_event.h"

// interfaces of various windows

#include "win32_evoHistWindow.h"
#include "win32_gridWindow.h"
#include "win32_editor.h"
#include "win32_displayOptions.h"
#include "win32_statistics.h"
#include "win32_historyInfo.h"
#include "win32_crsrWindow.h"
#include "win32_performanceWindow.h"
#include "win32_status.h"

// infrastructure

#include "win32_protocolServer.h"
#include "win32_scriptHook.h"
#include "win32_readBuffer.h"
#include "win32_colorManager.h"
#include "win32_winManager.h"
#include "win32_focusPoint.h"
#include "win32_workThreadInterface.h"
#include "EvoHistorySysGlue.h"
#include "GDI_driver.h"
#include "D3D_driver.h"

// application

#include "win32_appMenu.h"
#include "win32_evoController.h"

class Script;
class NextGenFunctor;
class ScriptHook;
class HistorySystem;
class GraphicsInterface;
class D3D_driver;
class EvoModelDataGlue;
class ViewCollection;
class EvolutionCore;
class AppMenu;

class AppWindow : public BaseWindow
{
public:
    AppWindow( );
	void Start( );
	void Stop( );
	~AppWindow( );

private:
    virtual LRESULT UserProc( UINT const, WPARAM const, LPARAM const );

	AppWindow             ( AppWindow const & );  // noncopyable class 
    AppWindow & operator= ( AppWindow const & );  // noncopyable class 

	void adjustChildWindows( );

	HWND        m_hwndApp;
	HWND        m_hwndConsole;
	Util::Event m_event;
	BOOL        m_bStopped;  // if true, no grid visible, many functions not available
	HCURSOR     m_hCrsrWait;

	FocusPoint          m_FocusPoint;
	WinManager          m_WinManager;
	ColorManager        m_ColorManager;
	StatusBar           m_StatusBar;
	CrsrWindow          m_CrsrWindow;
	HistInfoWindow      m_HistInfoWindow;
    EditorWindow        m_EditorWindow;
    StatisticsWindow    m_Statistics;
    DspOptWindow        m_DspOptWindow;
	GridWindow          m_MainGridWindow;
    GridWindow          m_MiniGridWindow;
	PerformanceWindow   m_PerfWindow;
	EvoHistWindow       m_EvoHistWindow;
	EvoController       m_EvoController;
	EvoHistorySysGlue   m_EvoHistGlue;
	ViewCollection      m_CoreObservers;
	D3D_driver          m_D3d_driver;
	ReadBuffer          m_ReadBuffer;
	AppMenu             m_AppMenu;
	WorkThreadInterface m_WorkThreadInterface;
    ScriptHook          m_ScriptHook;
	ProtocolServer      m_protocolServer;

	GraphicsInterface   * m_pGraphics;
	EvoModelDataGlue    * m_pModelDataWork;
	EvolutionCore       * m_pEvoCore4Display;
    HistorySystem       * m_pHistorySystem;

    std::wofstream m_traceStream;
};
