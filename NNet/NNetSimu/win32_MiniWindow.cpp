// win32_MiniWindow.cpp
//
// NNetWindows

#include "stdafx.h"
#include "MoreTypes.h"
#include "Resource.h"
#include "DrawModel.h"
#include "NNetColors.h"
#include "NNetParameters.h"
#include "NNetModelInterface.h"
#include "win32_MiniWindow.h"

void MiniWindow::Observe( NNetWindow * const pNNetWin )	
{ 
	m_pNNetWindowObserved = pNNetWin;
	m_context.SetNoColors( true );
}

void MiniWindow::OnMouseMove( WPARAM const wParam, LPARAM const lParam )
{
	if ( ! m_pNNetWindowObserved )
		return; 

	if ( wParam & MK_LBUTTON )       	// Left mouse button: move or edit action
	{
		PixelPoint const ptCrsr { GetCrsrPosFromLparam( lParam ) };  // relative to client area

		if ( m_ptLast.IsNotNull() )     // last cursor pos stored in m_ptLast
		{
			PixelPoint      const pixDelta { ptCrsr - m_ptLast };
			MicroMeterPoint const umDelta  { m_context.GetCoordC().Convert2MicroMeterPointSize( pixDelta ) }; 
			m_pNNetWindowObserved->NNetMove( - umDelta );               // move the observed window in opposite direction 
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
		float          const EXTRA_SPACE_FACTOR { 1.2f };   // give 20% more space (looks better)
		MicroMeterRect const umRectMain  { m_pNNetWindowObserved->GetViewRect() };   // current position of main window view 
		MicroMeterRect const umRectModel { m_pModelInterface->GetEnclosingRect() };  // current extension of model
		MicroMeterRect const umRectShow  { Union( umRectMain, umRectModel ) };       // all this should be visible  
		CenterAndZoomRect( umRectShow, EXTRA_SPACE_FACTOR, false );           
}

void MiniWindow::doPaint( ) 
{
	if ( m_pNNetWindowObserved )
	{
		adjust();
		m_pDrawModel->DrawExteriorInRect( GetClPixelRect( ), m_context );
		m_context.DrawTranspRect( m_pNNetWindowObserved->GetViewRect(), NNetColors::POSITION_RECT );
	}
}
