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
class StatisticsWindow;
class EditorWindow;
class DspOptWindow;
class EvoHistWindow;
class WorkThread;
class GridRect;
class Script;
class FocusPoint;
class WinManager;
class EvolutionCore;
class NextGenFunctor;
class HistWorkThread;
class ScriptHook;
class EvolutionModelData;
class EvoHistorySys;
class EvoController;

//lint -esym( 1712, AppWindow )  no default constructor for class 

class AppWindow : public BaseWindow
{
public:
    AppWindow( );
    void Start( HINSTANCE const, LPTSTR const );
    ~AppWindow( );

protected:
    virtual LRESULT UserProc( UINT const, WPARAM const, LPARAM const );

private:
    AppWindow             ( AppWindow const & );  // noncopyable class 
    AppWindow & operator= ( AppWindow const & );  // noncopyable class 

    void adjustChildWindows( );

    DisplayAll          m_displayGridFunctor;

    GridWindow         * m_pMainGridWindow;
    GridWindow         * m_pMiniGridWindow;
    WorkThread         * m_pWorkThread;
    PerformanceWindow  * m_pPerfWindow;
    EditorWindow       * m_pEditorWindow;
    CrsrWindow         * m_pCrsrWindow;
    StatusBar          * m_pStatusBar;
    StatisticsWindow   * m_pStatistics;
    DspOptWindow       * m_pDspOptWindow;
    GridRect           * m_pGridRectSel;
    FocusPoint         * m_pFocusPoint;
    WinManager         * m_pWinManager;
    EvolutionModelData * m_pModelWork;
    EvolutionCore      * m_pEvolutionCore;
	EvoController      * m_pEvoController;
    ScriptHook         * m_pScriptHook;
    
    // History 
	EvoHistorySys      * m_pEvoHistorySys;
    EvoHistWindow      * m_pEvoHistWindow;
    HistWorkThread     * m_pHistWorkThread;

    std::wofstream m_traceStream;
};
