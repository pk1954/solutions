// win32_appWindow.h : 
//

#pragma once

#include <fstream>
#include <iostream>
#include "win32_baseWindow.h"
#include "win32_event.h"
#include "win32_delay.h"
#include "win32_actionTimer.h"

// infrastructure

#include "win32_readBuffer.h"
#include "win32_winManager.h"

// application

#include "win32_NNetSimuMenu.h"
#include "win32_NNetSimuController.h"

class Script;
class AppMenu;

class NNetSimuWindow : public BaseWindow
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

	void adjustChildWindows( );

	HWND                m_hwndApp;
	HWND                m_hwndConsole;
	Util::Event         m_event;
	BOOL                m_bStarted;  // if true, model is visible, all functions available

	NNetSimuController  m_NNetSimuController;
	ActionTimer         m_atComputation;
	ActionTimer         m_atDisplay;
	Delay               m_Delay;
	WinManager          m_WinManager;
	NNetSimuMenu        m_AppMenu;

    std::wofstream m_traceStream;
};
