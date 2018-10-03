// win32_rootWindow.cpp
//

#include "stdafx.h"
#include "Observer.h"
#include "win32_rootWindow.h"

BOOL RootWinIsReady( RootWindow const * pRootWin )
{
    return ( ( pRootWin != nullptr ) && ( pRootWin->GetWindowHandle( ) != nullptr ) );
}

RootWindow::RootWindow( ) : 
	m_hwnd( nullptr ),
	m_hwndApp( nullptr ),
	m_pObserver( nullptr ),
	m_iDisplayRate( 0 )
{ }

RootWindow::~RootWindow( ) 
{ 
	delete m_pObserver;

	m_pObserver = nullptr;
	m_hwnd      = nullptr; 
}

void RootWindow::SetWindowHandle( HWND const hwnd ) 
{ 
	m_hwnd      = hwnd;  
	m_pObserver = new Observer( this );
	m_hwndApp   = GetAncestor( m_hwnd, GA_ROOTOWNER );
};

ObserverInterface * RootWindow::GetObserver() const 
{ 
	return m_pObserver; 
}
