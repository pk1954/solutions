// EvoAppWindow.ixx
//
// Evolution

module;

import std;
import D3D_driver;
import Event;

// infrastructure

import WppWindowInterface;
import ProtocolServer;
import ScriptHook;
import ColorManager;
import EvoWorkThreadInterface;
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
import GraphicsInterface;
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

private:
	EvoAppWindow             (EvoAppWindow const &);  // noncopyable class 
    EvoAppWindow & operator= (EvoAppWindow const &);  // noncopyable class 

	virtual void ProcessAppCommand(WPARAM const, LPARAM const = 0);

	void configureStatusBar();

	Util::Event              m_eventPOI;
					         
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
