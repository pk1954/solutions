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
#include "win32_readBuffer.h"
#include "win32_winManager.h"
#include "win32_NNetWorkThreadInterface.h"

// application

#include "NNetHistorySysGlue.h"
#include "win32_NNetAppMenu.h"
#include "win32_NNetReadBuffer.h"
#include "win32_NNetSimuController.h"

class Script;
class AppMenu;
class StatusBar;
class ModelWindow;
class NNetModel;
class NNetWindow;
class HistInfoWindow;

class NNetSimuWindow : public BaseAppWindow
{
public:
	NNetSimuWindow( );
	void Start( );
	void Stop( );
	~NNetSimuWindow( );

private:
	NNetSimuWindow             ( NNetSimuWindow const & );  // noncopyable class 
	NNetSimuWindow & operator= ( NNetSimuWindow const & );  // noncopyable class 

	Util::Event m_event;

	NNetWindow             * m_pMainNNetWindow;
	HistInfoWindow         * m_pHistInfoWindow;
	NNetModelDataGlue      * m_pModelDataWork;
	NNetModel              * m_pNNetModel4Display;

	NNetWorkThreadInterface m_NNetWorkThreadInterface;
	NNetSimuController      m_NNetSimuController;
	ActionTimer             m_atComputation;
	ActionTimer             m_atDisplay;
	Delay                   m_Delay;
	NNetReadBuffer          m_NNetReadBuffer;
	NNetHistorySysGlue      m_NNetHistGlue;

    std::wofstream m_traceStream;
};
