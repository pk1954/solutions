// win32_MainWindow.h : 
//
// NNetWindows

#pragma once

#include "win32_NNetWindow.h"

class MainWindow : public NNetWindow
{
public:

	void AddObserver( ObserverInterface * const pObs ) 
	{ 
		m_pObserver = pObs;
	}

	virtual bool Zoom( MicroMeter const newSize, PixelPoint const * const pPixPntCenter )
	{
		bool bRes { NNetWindow::Zoom( newSize, pPixPntCenter ) };
		if ( bRes )
			m_pObserver->Notify( false );
		return bRes;
	}

	void NNetMove( PixelPoint const & pixDelta ) 
	{ 
		NNetWindow::NNetMove( pixDelta );
		m_pObserver->Notify( false );
	}

	virtual void smoothStep( ) 
	{ 
		NNetWindow::smoothStep( );
		m_pObserver->Notify( false );
	}

	void OnSize( WPARAM const wParam, LPARAM const lParam )
	{
		NNetWindow::OnSize( wParam, lParam );
		m_pObserver->Notify( false );
	}

private:

	ObserverInterface * m_pObserver {}; 
};

