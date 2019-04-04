// win32_appWindow.h : 
//

#pragma once

#include <fstream>
#include <iostream>
#include "win32_baseWindow.h"
#include "win32_event.h"

class GridWindow;
class PerformanceWindow;
class StatusBar;
class CrsrWindow;
class HistInfoWindow;
class StatisticsWindow;
class EditorWindow;
class DspOptWindow;
class EvoHistWindow;
class WorkThreadInterface;
class Script;
class FocusPoint;
class WinManager;
class EvolutionCore;
class NextGenFunctor;
class ScriptHook;
class EvoHistorySysGlue;
class EvoController;
class HistorySystem;
class ColorManager;
class GraphicsInterface;
class ReadBuffer;
class D3D_driver;

//lint -esym( 1712, AppWindow )  no default constructor for class 

class AppWindow : public BaseWindow
{
public:
    AppWindow( );
    void Start( );
    ~AppWindow( );

private:
    virtual LRESULT UserProc( UINT const, WPARAM const, LPARAM const );

	AppWindow             ( AppWindow const & );  // noncopyable class 
    AppWindow & operator= ( AppWindow const & );  // noncopyable class 

	void adjustChildWindows( );
	void adjustMiniWinVisibility( int const );
	void shutDown( );

	HWND        m_hwndApp;
	HWND        m_hwndConsole;
	Util::Event m_event;

	D3D_driver          * m_pD3d_driver;
	GridWindow          * m_pMainGridWindow;
    GridWindow          * m_pMiniGridWindow;
    WorkThreadInterface * m_pWorkThreadInterface;
    PerformanceWindow   * m_pPerfWindow;
    EditorWindow        * m_pEditorWindow;
    CrsrWindow          * m_pCrsrWindow;
	HistInfoWindow      * m_pHistInfoWindow;
    StatusBar           * m_pStatusBar;
    StatisticsWindow    * m_pStatistics;
    DspOptWindow        * m_pDspOptWindow;
    FocusPoint          * m_pFocusPoint;
    WinManager          * m_pWinManager;
    EvolutionCore       * m_pEvolutionCoreWork;
	EvoController       * m_pEvoController;
    ScriptHook          * m_pScriptHook;
	ColorManager        * m_pColorManager;
	GraphicsInterface   * m_pGraphics;
	ReadBuffer          * m_pReadBuffer;
    
    // History 
    HistorySystem       * m_pHistorySystem;
	EvoHistorySysGlue   * m_pEvoHistGlue;
    EvoHistWindow       * m_pEvoHistWindow;

    std::wofstream m_traceStream;
};
