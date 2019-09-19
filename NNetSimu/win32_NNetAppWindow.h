// win32_NNetAppWindow.h : 
//
// NNetSimu

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

class NNetAppWindow : public BaseAppWindow
{
public:
	NNetAppWindow( );
	void Start( );
	void Stop( );
	~NNetAppWindow( );

private:
	NNetAppWindow             ( NNetAppWindow const & );  // noncopyable class 
	NNetAppWindow & operator= ( NNetAppWindow const & );  // noncopyable class 

	virtual void ProcessAppCommand( WPARAM const, LPARAM const = 0 );

	Util::Event              m_eventPOI;

	NNetWorkThreadInterface m_NNetWorkThreadInterface;
	ActionTimer             m_atComputation;
	ActionTimer             m_atDisplay;
	Delay                   m_Delay;
	NNetReadBuffer          m_NNetReadBuffer;
	NNetHistorySysGlue      m_NNetHistGlue;

	NNetSimuController    * m_pNNetSimuController;
	NNetModel             * m_pNNetModel4Display;
	NNetWindow            * m_pMainNNetWindow;
	NNetModelDataGlue     * m_pModelDataWork;
};
