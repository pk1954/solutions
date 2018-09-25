// win32_appWindow.h : 
//

#pragma once

#include <fstream>
#include <iostream>
#include "win32_baseWindow.h"
#include "win32_displayAll.h"

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
class GridRect;
class Script;
class FocusPoint;
class WinManager;
class EvolutionCore;
class NextGenFunctor;
class ScriptHook;
class EvoHistorySysGlue;
class EvoController;

//lint -esym( 1712, AppWindow )  no default constructor for class 

class AppWindow : public BaseWindow
{
public:
    AppWindow( );
    void Start( HINSTANCE const, LPTSTR const );
    ~AppWindow( );

private:
    virtual LRESULT UserProc( UINT const, WPARAM const, LPARAM const );

	AppWindow             ( AppWindow const & );  // noncopyable class 
    AppWindow & operator= ( AppWindow const & );  // noncopyable class 

	void adjustChildWindows( );

    DisplayAll            m_displayGridFunctor;

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
    EvolutionCore       * m_pEvolutionCore;
	EvoController       * m_pEvoController;
    ScriptHook          * m_pScriptHook;
    
    // History 
	EvoHistorySysGlue   * m_pEvoHistGlue;
    EvoHistWindow       * m_pEvoHistWindow;

    std::wofstream m_traceStream;
};
