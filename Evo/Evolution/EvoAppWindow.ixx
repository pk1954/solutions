// EvoAppWindow.ixx
//
// Evolution

export module EvoAppWindow;

import std;
import D3D_driver;
import Event;

// infrastructure

import WppWindowInterface;
import ProtocolServer;
import ScriptHook;
import ColorManager;
import EvoWorkThread;
import EvoHistorySysGlue;
import EvoReadBuffer;

// application

import EvoAppMenu;
import EvoController;
import RunTime;
import FocusPoint;
import GridWindow;
import EvoEditorWindow;
import StatisticsWindow;
import CrsrWindow;
import PerformanceWindow;
import GenerationDisplay;
import StatusBar;
import ScriptHook;
import D3D_driver;
import EvoModelDataGlue;
import EvolutionCore;
import DspOptWindow;
import EvoController;
import AppMenu;

export class EvoAppWindow : public AppWindowInterface
{
public:
    EvoAppWindow();
	~EvoAppWindow();

	virtual void Start();
	virtual void Stop();

	void DoGameStuff() { }

private:
	EvoAppWindow             (EvoAppWindow const &);  // noncopyable class 
    EvoAppWindow & operator= (EvoAppWindow const &);  // noncopyable class 

	virtual void ProcessAppCommand(WPARAM const, LPARAM const = 0);

	void configureStatusBar();

	Event              m_eventPOI;
	EvoHistorySysGlue  m_EvoHistGlue;
	ColorManager       m_ColorManager;
	D3D_driver         m_D3d_driver;
    ScriptHook         m_ScriptHook;
	ProtocolServer     m_protocolServer;
					       
	EvoWorkThread     * m_pEvoWorkThread;
	EvoReadBuffer     * m_pEvoReadBuffer;
	EvoController     * m_pEvoController;
	EvolutionCore     * m_pEvoCore4Display;
	GridWindow        * m_pMainGridWindow;
	EvoModelDataGlue  * m_pModelDataWork;
					  
    GridWindow        * m_pMiniGridWindow;
    EvoEditorWindow   * m_pEvoEditorWindow;
	DspOptWindow      * m_pDspOptWindow;
	PerformanceWindow * m_pPerfWindow;
	CrsrWindow        * m_pCrsrWindow;
	StatisticsWindow  * m_pStatistics;
	D3D_driver        * m_pGraphics;
	FocusPoint        * m_pFocusPoint;
	GenerationDisplay * m_pGenerationDisplay;
};
