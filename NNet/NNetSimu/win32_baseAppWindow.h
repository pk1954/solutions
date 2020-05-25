// win32_baseAppWindow.h
//
// NNetWindows

#pragma once

#include <fstream>
#include <iostream>
#include "DisplayFunctor.h"
#include "win32_baseWindow.h"
#include "win32_winManager.h"
#include "win32_status.h"

class ComputeThread;
class ModelWindow;
class NNetAppMenu;

using std::wofstream;

class BaseAppWindow : public BaseWindow
{
public:
	virtual ~BaseAppWindow() {}; 

	void Initialize( );

	void Start
	( 
		ModelWindow   * const,
		ComputeThread * const 
	);

	virtual bool OnCommand( WPARAM const, LPARAM const, PixelPoint const );
	virtual void OnClose( ) = 0;
	virtual void Stop( );

protected:
	HWND          m_hwndApp    { nullptr };
	NNetAppMenu * m_pAppMenu   { nullptr };        // allocated by application
	WinManager    m_WinManager { };
	StatusBar     m_StatusBar  { };

private:

	HWND m_hwndConsole { nullptr };

	ModelWindow   * m_pModelWindow   { nullptr };
	ComputeThread * m_pComputeThread { nullptr };

	void adjustChildWindows( );

	virtual LRESULT UserProc( UINT const, WPARAM const, LPARAM const );
};	

class StatusBarDisplayFunctor : public DisplayFunctor
{
public:
	StatusBarDisplayFunctor( StatusBar * const pStatusBar, int const iPart )
	  : m_wstrBuffer( ),
		m_pStatusBar(pStatusBar),
		m_iPart( iPart)
	{ }

	virtual void operator() ( wstring const & line )
	{
		m_wstrBuffer = line;
		m_pStatusBar->DisplayInPart( m_iPart, m_wstrBuffer );
	}

private:
	wstring     m_wstrBuffer;
	StatusBar * m_pStatusBar;
	int         m_iPart;
};
