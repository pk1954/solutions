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

	MicroMeterRect const GetViewRect() 
	{ 
		return m_context.GetCoord().Convert2MicroMeterRect( GetClPixelRect() ); 
	};

	virtual bool Zoom( MicroMeter const newSize )
	{
		bool bRes { NNetWindow::Zoom( newSize ) };
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

	virtual void NNetMove( MicroMeterPoint const & umDelta )	
	{ 
		NNetMove( m_context.GetCoord().Convert2PixelSize( umDelta ) ); 
	}

	void OnSize( WPARAM const wParam, LPARAM const lParam )
	{
		NNetWindow::OnSize( wParam, lParam );
		m_pObserver->Notify( false );
	}

private:

	ObserverInterface * m_pObserver {}; 
};

