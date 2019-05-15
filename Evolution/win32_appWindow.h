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

#include "win32_colorManager.h"
#include "win32_winManager.h"
#include "win32_focusPoint.h"

class WorkThreadInterface;
class Script;
class NextGenFunctor;
class ScriptHook;
class EvoHistorySysGlue;
class EvoController;
class HistorySystem;
class GraphicsInterface;
class ReadBuffer;
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
	void adjustMiniWinVisibility( int const );

	HWND        m_hwndApp;
	HWND        m_hwndConsole;
	Util::Event m_event;
	BOOL        m_bStopped;  // if true, no grid visible, many functions not available

	FocusPoint        m_FocusPoint;
	WinManager        m_WinManager;
	ColorManager      m_ColorManager;
	StatusBar         m_StatusBar;
	CrsrWindow        m_CrsrWindow;
	HistInfoWindow    m_HistInfoWindow;
    EditorWindow      m_EditorWindow;
    StatisticsWindow  m_Statistics;
    DspOptWindow      m_DspOptWindow;
	GridWindow        m_MainGridWindow;
    GridWindow        m_MiniGridWindow;
	PerformanceWindow m_PerfWindow;

	EvoHistWindow     m_EvoHistWindow;

	D3D_driver          * m_pD3d_driver;
    WorkThreadInterface * m_pWorkThreadInterface;
	EvoController       * m_pEvoController;
    ScriptHook          * m_pScriptHook;
	GraphicsInterface   * m_pGraphics;
	ReadBuffer          * m_pReadBuffer;
	EvoModelDataGlue    * m_pModelDataWork;
	ViewCollection      * m_pCoreObservers;
	EvolutionCore       * m_pEvoCore4Display;
	AppMenu             * m_pAppMenu;
    
    // History 
    HistorySystem       * m_pHistorySystem;
	EvoHistorySysGlue   * m_pEvoHistGlue;

    std::wofstream m_traceStream;
};
