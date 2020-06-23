// win32_MiniWindow.cpp
//
// NNetWindows

#include "stdafx.h"
#include "MoreTypes.h"
#include "Resource.h"
#include "DrawModel.h"
#include "NNetColors.h"
#include "NNetParameters.h"
#include "win32_MainWindow.h"
#include "win32_MiniWindow.h"

void MiniWindow::ObservedNNetWindow( MainWindow * const pNNetWin )	
{ 
	m_pObservedNNetWindow = pNNetWin;
	m_context.SetNoColors( true );
}

void MiniWindow::OnMouseMove( WPARAM const wParam, LPARAM const lParam )
{
	if ( ! m_pObservedNNetWindow )
		return; 

	if ( wParam & MK_LBUTTON )       	// Left mouse button: move or edit action
	{
		PixelPoint const ptCrsr { GetCrsrPosFromLparam( lParam ) };  // screen coordinates

		if ( m_ptLast.IsNotNull() )     // last cursor pos stored in m_ptLast
		{
			PixelPoint      const pixDelta { ptCrsr - m_ptLast };
			MicroMeterPoint const umDelta  { m_context.GetCoordC().Convert2MicroMeterPointSize( pixDelta ) }; 
			m_pObservedNNetWindow->NNetMove( - umDelta );               // move the observed window in opposite direction 
		}

		m_ptLast = ptCrsr;
	}
	else
	{
		m_ptLast.Set2Null();   // make m_ptLast invalid
	}
}

void MiniWindow::adjust( ) 
{
	float          const EXTRA_SPACE_FACTOR { 1.2f };                          // give 20% more space (looks better)
	MicroMeterRect const umRectMain  { m_pObservedNNetWindow->GetViewRect() }; // current position of main window view 
	MicroMeterRect const umRectModel { GetEnclosingRect() };                   // current extension of model
	MicroMeterRect const umRectShow  { Union( umRectMain, umRectModel ) };     // all this should be visible  
	CenterAndZoomRect( umRectShow, EXTRA_SPACE_FACTOR, false );           
}

void MiniWindow::doPaint( ) 
{
	if ( m_pObservedNNetWindow )
	{
		adjust();
		GetDrawModel()->DrawExteriorInRect( GetClPixelRect( ), m_context );
		m_context.DrawTranspRect( m_pObservedNNetWindow->GetViewRect(), NNetColors::POSITION_RECT );
	}
}
