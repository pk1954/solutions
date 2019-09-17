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
#include "win32_NNetSimuMenu.h"
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
    virtual LRESULT UserProc( UINT const, WPARAM const, LPARAM const );

	NNetSimuWindow             ( NNetSimuWindow const & );  // noncopyable class 
	NNetSimuWindow & operator= ( NNetSimuWindow const & );  // noncopyable class 

	HWND                m_hwndApp;
	Util::Event         m_event;
	BOOL                m_bStarted;  // if true, model is visible, all functions available

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
	NNetSimuMenu            m_AppMenu;

    std::wofstream m_traceStream;
};
