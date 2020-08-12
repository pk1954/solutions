// win32_NNetWindow.cpp
//
// NNetWindows

#include "stdafx.h"
#include <sstream> 
#include "Resource.h"
#include "MoreTypes.h"
#include "Segment.h"
#include "scale.h"
#include "Pipe.h"
#include "InputNeuron.h"
#include "PixelTypes.h"
#include "NNetParameters.h"
#include "NNetModelReaderInterface.h"
#include "NNetColors.h"
#include "DrawModel.h"
#include "win32_sound.h"
#include "win32_tooltip.h"
#include "NNetModelWriterInterface.h"
#include "win32_NNetWindow.h"

using std::function;

void NNetWindow::InitClass( ActionTimer * const pActionTimer )
{
	ModelWindow::InitClass( pActionTimer );
}

NNetWindow::NNetWindow( ) :
	ModelWindow( )
{ }

void NNetWindow::Start
( 
	HWND                       const hwndApp, 
	DWORD                      const dwStyle,
	bool                       const bShowRefreshRateDialog,
	NNetController           * const pController,
	NNetModelReaderInterface * const pModelReaderInterface,
	NNetModelWriterInterface * const pModelWriterInterface,
	DrawModel                * const pDrawModel,
	Observable               * const pCursorObservable
)
{
	m_pDrawModel = pDrawModel;
	HWND hwnd = StartBaseWindow
	( 
		hwndApp,
		CS_OWNDC | CS_DBLCLKS,
		L"ClassNNetWindow",
		dwStyle,
		nullptr,
		nullptr
	);
	m_context.Start( hwnd );
	m_pController           = pController;
	m_pModelReaderInterface = pModelReaderInterface;
	m_pModelWriterInterface = pModelWriterInterface;
	m_pCursorPosObservable  = pCursorObservable;
	ShowRefreshRateDlg( bShowRefreshRateDialog );
}

void NNetWindow::Reset( )
{ 
	m_shapeHighlighted      = NO_SHAPE; 
	m_shapeSuperHighlighted = NO_SHAPE; 
}

void NNetWindow::Stop( )
{
	Reset( );
	m_context.Stop();
	DestroyWindow( );
}

NNetWindow::~NNetWindow( )
{
	m_pModelReaderInterface = nullptr;
	m_pModelWriterInterface = nullptr;
	m_pCursorPosObservable  = nullptr;
	m_pDrawModel            = nullptr;
	m_pController           = nullptr;
}

bool NNetWindow::Zoom( MicroMeter const newSize, PixelPoint const * const pPixPntCenter )
{
	PixelPoint const pixPntCenter { pPixPntCenter ? * pPixPntCenter : GetClRectCenter() };
	bool bRes { m_context.ZoomKeepCrsrPos( pixPntCenter, newSize ) };
	if ( bRes ) 
		Notify( false ); 
	return bRes;
}

void NNetWindow::ZoomStep( bool const bZoomIn, PixelPoint const * const pPixPntCenter )
{
	Zoom( m_context.GetCoordC().ComputeNewPixelSize( bZoomIn ), pPixPntCenter );
}

void NNetWindow::NNetMove( PixelPoint const & pixDelta ) 
{ 
	m_context.Move( pixDelta ); 
	Notify( true );  
}

long NNetWindow::AddContextMenuEntries( HMENU const hPopupMenu )
{
	if ( m_pModelReaderInterface->AnyShapesSelected( ) )
	{
		AppendMenu( hPopupMenu, MF_STRING, IDM_DESELECT_ALL,     L"Deselect all" );
		AppendMenu( hPopupMenu, MF_STRING, IDM_COPY_SELECTION,   L"Copy selection" );
		AppendMenu( hPopupMenu, MF_STRING, IDM_MARK_SELECTION,   L"Mark selection" );
		AppendMenu( hPopupMenu, MF_STRING, IDM_UNMARK_SELECTION, L"Unmark selection" );
		AppendMenu( hPopupMenu, MF_STRING, IDM_DELETE_SELECTION, L"Delete selected objects" );
		AppendMenu( hPopupMenu, MF_STRING, IDM_CLEAR_BEEPERS,    L"Clear selected trigger sounds" );
	}
	else if ( IsUndefined( m_shapeHighlighted ) )  // no shape highlighted, cursor on background
	{
		AppendMenu( hPopupMenu, MF_STRING, IDD_NEW_NEURON,         L"New neuron" );
		AppendMenu( hPopupMenu, MF_STRING, IDD_NEW_INPUT_NEURON,   L"New input neuron" );
		AppendMenu( hPopupMenu, MF_STRING, IDM_SELECT_ALL_BEEPERS, L"Select all neurons with trigger sound" );
		AppendMenu( hPopupMenu, MF_STRING, IDM_CLEAR_BEEPERS,      L"Clear all trigger sounds" );
	}
	else switch ( m_pModelReaderInterface->GetShapeType( m_shapeHighlighted ).GetValue() )
	{
	case ShapeType::Value::inputNeuron:
		if ( ! m_pModelReaderInterface->HasOutgoing( m_shapeHighlighted ) )
			AppendMenu( hPopupMenu, MF_STRING, IDD_ADD_OUTGOING2KNOT, L"Add outgoing dendrite" );
		AppendMenu( hPopupMenu, MF_STRING, IDD_PULSE_RATE,            L"Pulse rate" );
		AppendMenu( hPopupMenu, MF_STRING, IDD_DELETE_SHAPE,          L"Delete" );
		AppendMenu( hPopupMenu, MF_STRING, IDD_DISCONNECT,            L"Disconnect" );
		break;

	case ShapeType::Value::neuron:
		if ( ! m_pModelReaderInterface->HasOutgoing( m_shapeHighlighted ) )
			AppendMenu( hPopupMenu, MF_STRING, IDD_ADD_OUTGOING2KNOT, L"Add outgoing dendrite" );
		AppendMenu( hPopupMenu, MF_STRING, IDD_ADD_INCOMING2KNOT,     L"Add incoming dendrite" );
		AppendMenu( hPopupMenu, MF_STRING, IDD_DELETE_SHAPE,          L"Delete" );
		AppendMenu( hPopupMenu, MF_STRING, IDD_DISCONNECT,            L"Disconnect" );
		AppendMenu( hPopupMenu, MF_STRING, IDD_TRIGGER_SOUND_DLG,     L"Trigger sound" );
		AppendMenu( hPopupMenu, MF_STRING, IDM_SELECT_SUBTREE,        L"Select subtree" );
		AppendMenu( hPopupMenu, MF_STRING, IDD_STOP_ON_TRIGGER,       L"Stop on trigger on/off" );
		break;

	case ShapeType::Value::knot:  
		AppendMenu( hPopupMenu, MF_STRING, IDD_ADD_OUTGOING2KNOT, L"Add outgoing dendrite" );
		AppendMenu( hPopupMenu, MF_STRING, IDD_ADD_INCOMING2KNOT, L"Add incoming dendrite" );
		if ( 
				(! m_pModelReaderInterface->HasOutgoing( m_shapeHighlighted )) || 
				(
					! m_pModelReaderInterface->HasIncoming( m_shapeHighlighted ) && 
					( m_pModelReaderInterface->GetNrOfOutgoingConnections( m_shapeHighlighted ) <= 1 )  
				) 
			)
			AppendMenu( hPopupMenu, MF_STRING, IDD_APPEND_NEURON, L"Add neuron" );
		if ( 
				( m_pModelReaderInterface->GetNrOfOutgoingConnections( m_shapeHighlighted ) <= 1 ) && 
				(! m_pModelReaderInterface->HasIncoming( m_shapeHighlighted )) 
			)
			AppendMenu( hPopupMenu, MF_STRING, IDD_APPEND_INPUT_NEURON, L"Add input neuron" );
		AppendMenu( hPopupMenu, MF_STRING, IDD_DISCONNECT, L"Disconnect" );
		break;

	case ShapeType::Value::pipe:
		AppendMenu( hPopupMenu, MF_STRING, IDD_ADD_OUTGOING2PIPE, L"Add outgoing dendrite" );
		AppendMenu( hPopupMenu, MF_STRING, IDD_ADD_INCOMING2PIPE, L"Add incoming dendrite" );
		AppendMenu( hPopupMenu, MF_STRING, IDD_INSERT_NEURON,     L"Insert neuron" );
		AppendMenu( hPopupMenu, MF_STRING, IDD_DELETE_SHAPE,      L"Delete" );
		if ( Pipe::GetArrowSize( ) > 0.0_MicroMeter )
			AppendMenu( hPopupMenu, MF_STRING, IDD_ARROWS_OFF,    L"Arrows off" );
		else
			AppendMenu( hPopupMenu, MF_STRING, IDD_ARROWS_ON,     L"Arrows on" );
		break;

	default:
		assert( false );
	}

	if ( IsDefined( m_shapeHighlighted ) )
	{
		if ( m_pModelReaderInterface->IsSelected( m_shapeHighlighted ) )
			AppendMenu( hPopupMenu, MF_STRING, IDM_DESELECT_SHAPE, L"Deselect" );
		else
			AppendMenu( hPopupMenu, MF_STRING, IDM_SELECT_SHAPE, L"Select" );
	}	

	return m_shapeHighlighted.GetValue(); // will be forwarded to HandleContextMenuCommand
}

void NNetWindow::OnMouseMove( WPARAM const wParam, LPARAM const lParam )
{
	PixelPoint      const ptCrsr    { GetCrsrPosFromLparam( lParam ) };  // screen coordinates
	MicroMeterPoint const umCrsrPos { m_context.GetCoord().Convert2MicroMeterPointPos( ptCrsr   ) };
	MicroMeterPoint const umLastPos { m_context.GetCoord().Convert2MicroMeterPointPos( m_ptLast ) };

	if ( m_pCursorPosObservable )
		m_pCursorPosObservable->NotifyAll( false );

	if ( wParam & MK_RBUTTON )         // Right mouse button: selection
	{
		if ( m_ptLast.IsNotNull() )    // last cursor pos stored in m_ptLast
		{
			m_rectSelection = MicroMeterRect( umCrsrPos, umLastPos );
		}
		else                           // first time here after RBUTTON pressed
		{
			m_ptLast = ptCrsr;         // store current cursor pos
		}
	}
	else if ( wParam & MK_LBUTTON )  	// Left mouse button: move or edit action
	{
		if ( m_ptLast.IsNotNull() )     // last cursor pos stored in m_ptLast
		{
			m_shapeSuperHighlighted = NO_SHAPE;
			if ( IsDefined( m_shapeHighlighted ) )
			{
				setSuperHighlightedShape( m_pModelReaderInterface->GetShapePos( m_shapeHighlighted ) );
				m_pModelWriterInterface->MoveShape( m_shapeHighlighted, umCrsrPos - umLastPos );
			}
			else if ( m_pModelReaderInterface->AnyShapesSelected( ) )   // move selected shapes 
			{
				m_pModelWriterInterface->MoveSelection( umCrsrPos - umLastPos );
			}
			else  // move view by manipulating coordinate system 
			{
				NNetMove( ptCrsr - m_ptLast );
			}
		}
		m_ptLast = ptCrsr;
	}
	else  // no mouse button pressed
	{                         
		setHighlightedShape( umCrsrPos );
		m_ptLast.Set2Null();   // make m_ptLast invalid
	}
}

void NNetWindow::setHighlightedShape( MicroMeterPoint const & umCrsrPos )
{
	ShapeId const idHighlight { m_pModelReaderInterface->FindShapeAt( umCrsrPos, ShapeCritAlwaysTrue ) };
	if ( idHighlight != m_shapeHighlighted )
	{
		m_shapeHighlighted = idHighlight; 
		Notify( true );     // cause immediate repaint
	}
}

void NNetWindow::setSuperHighlightedShape( MicroMeterPoint const & umCrsrPos )
{
	if ( m_pModelReaderInterface->IsOfType<BaseKnot>( m_shapeHighlighted ) )
	{
		m_shapeSuperHighlighted = m_pModelReaderInterface->FindShapeAt
		( 
			umCrsrPos,
			[&]( Shape const & shape ) { return m_pModelReaderInterface->ConnectsTo( m_shapeHighlighted, shape.GetId() ); } 
		);
	}
}

MicroMeterRect const NNetWindow::GetViewRect() const 
{ 
	return m_context.GetCoordC().Convert2MicroMeterRect( GetClPixelRect() ); 
};

tHighlightType const NNetWindow::GetHighlightType( Shape const & shape ) const
{
	return ( shape.GetId() == m_shapeSuperHighlighted )
		   ? tHighlightType::superHighlighted
		   : ( shape.GetId() == m_shapeHighlighted )
			 ? tHighlightType::highlighted
			 : tHighlightType::normal;
}

void NNetWindow::doPaint( ) 
{
	PixelRect const pixRect { GetClPixelRect( ) };
	
	if ( m_rectSelection.IsNotEmpty( ) )
		m_context.DrawTranspRect( m_rectSelection, NNetColors::SELECTION_RECT );

	if ( m_context.GetPixelSize() <= 5._MicroMeter )
		m_pDrawModel->DrawExteriorInRect( pixRect, m_context );

	m_pDrawModel->DrawInteriorInRect( pixRect, m_context, [&](Shape const & s) { return s.IsPipe    (); } );
	m_pDrawModel->DrawInteriorInRect( pixRect, m_context, [&](Shape const & s) { return s.IsBaseKnot(); } );

	if ( IsDefined( m_shapeSuperHighlighted ) ) // draw super highlighted shape again to be sure that it is in foreground
	{
		m_pModelReaderInterface->DrawExterior( m_shapeSuperHighlighted, m_context, tHighlightType::superHighlighted );
		m_pModelReaderInterface->DrawInterior( m_shapeSuperHighlighted, m_context );
	}

	if ( IsDefined( m_shapeHighlighted ) )  // draw highlighted shape again to be sure that it is in foreground
	{
		m_pModelReaderInterface->DrawExterior( m_shapeHighlighted, m_context, tHighlightType::highlighted );
		m_pModelReaderInterface->DrawInterior( m_shapeHighlighted, m_context );
	}

	m_context.ShowScale( GetClientWindowHeight() );

	if ( m_context.GetPixelSize() <= 2.5_MicroMeter )
		m_pDrawModel->DrawNeuronTextInRect( pixRect, m_context );
}

void NNetWindow::CenterModel( bool const bSmooth )
{
	CenterAndZoomRect( m_pModelReaderInterface->GetEnclosingRect( ), 1.2f, bSmooth ); // give 20% more space (looks better)
}

void NNetWindow::CenterAndZoomRect( MicroMeterRect const & umRect, float const fRatioFactor, bool const bSmooth )
{
	MicroMeterRect  const umRectScaled      { umRect.Scale( NEURON_RADIUS ) }; // give some more space to include complete shapes 
	float           const fVerticalRatio    { umRectScaled.GetHeight() / m_context.GetCoordC().Convert2MicroMeter( GetClientWindowHeight() ) };
	float           const fHorizontalRatio  { umRectScaled.GetWidth () / m_context.GetCoordC().Convert2MicroMeter( GetClientWindowWidth() ) };
	float           const fMaxRatio         { max( fVerticalRatio, fHorizontalRatio ) };
	float           const fDesiredRatio     { fMaxRatio * fRatioFactor };
	PixelPoint      const pixPointCenter    { GetClRectCenter( ) };
	fPixelPoint     const fpCenter          { m_context.GetCoordC().Convert2fPixelPoint( pixPointCenter ) };
	MicroMeter      const umPixelSizeTarget { m_context.GetCoordC().LimitPixelSize( m_context.GetCoordC().GetPixelSize() * fDesiredRatio ) };
	MicroMeterPoint const umPntCenterTarget { umRectScaled.GetCenter() };
	if ( bSmooth )
	{
		m_umPixelSizeStart = m_context.GetCoordC().GetPixelSize();                                // actual pixel size 
		m_umPntCenterStart = m_context.GetCoordC().Convert2MicroMeterPointPos( pixPointCenter );  // actual center 
		m_umPixelSizeDelta = umPixelSizeTarget - m_umPixelSizeStart;
		m_umPntCenterDelta = umPntCenterTarget - m_umPntCenterStart;
		m_smoothMove.Reset();
		m_bFocusMode = true;
	}
	else
	{
		m_context.Zoom( umPixelSizeTarget );
		m_context.Center( umPntCenterTarget, fpCenter );
	}
}

void NNetWindow::smoothStep( ) 
{
	float fPos            { m_smoothMove.Next() };
	bool  fTargetsReached { fPos >= SmoothMoveFp::END_POINT };

	if ( fTargetsReached )
	{
		m_bFocusMode = false;
		SendCommand2Application( IDM_CENTERING_FINISHED, 0	);
	}
	else
	{
		fPixelPoint const fpCenter { m_context.GetCoordC().Convert2fPixelPoint( GetClRectCenter( ) ) };
		m_context.Zoom  ( m_umPixelSizeStart + m_umPixelSizeDelta * fPos );
		m_context.Center( m_umPntCenterStart + m_umPntCenterDelta * fPos, fpCenter );
	}

	Notify( true );     // cause immediate repaint
}

void NNetWindow::OnPaint( )
{
	if ( IsWindowVisible() )
	{
		PAINTSTRUCT ps;
		HDC const hDC = BeginPaint( &ps );
		if ( m_context.StartFrame( hDC ) )
		{
			doPaint( );
			m_context.EndFrame( );
		}
		EndPaint( &ps );
	}

	if ( m_bFocusMode )
		smoothStep( );
}

void NNetWindow::OnSize( WPARAM const wParam, LPARAM const lParam )
{
	UINT width  = LOWORD(lParam);
	UINT height = HIWORD(lParam);
	m_context.Resize( width, height );
	Notify( false );
}

void NNetWindow::OnLeftButtonDblClick( WPARAM const wParam, LPARAM const lParam )
{
	if ( IsDefined( m_shapeHighlighted ) )
		m_pModelWriterInterface->SelectShape( m_shapeHighlighted, tBoolOp::opToggle );
}

void NNetWindow::OnMouseWheel( WPARAM const wParam, LPARAM const lParam )
{  
	PixelPoint const ptCrsr     { GetRelativeCrsrPosition() };  // screen coordinates
	int        const iDelta     { GET_WHEEL_DELTA_WPARAM( wParam ) / WHEEL_DELTA };
	bool       const bDirection { iDelta > 0 };

	for ( int iSteps = abs( iDelta ); iSteps > 0; --iSteps )
	{
		ZoomStep( bDirection, & ptCrsr );
	}
}

void NNetWindow::OnLButtonUp( WPARAM const wParam, LPARAM const lParam )
{
	if ( IsDefined( m_shapeHighlighted ) && IsDefined( m_shapeSuperHighlighted ) )
	{ 
		SendCommand2Application( IDD_CONNECT, 0	);
		m_shapeSuperHighlighted = NO_SHAPE;
	}
}

bool NNetWindow::OnRButtonUp( WPARAM const wParam, LPARAM const lParam )
{
	bool bMadeSelection { m_rectSelection.IsNotEmpty() };
	if ( bMadeSelection )
	{
		m_pModelWriterInterface->SelectShapesInRect( m_rectSelection );
		m_rectSelection.SetZero();
	}
	return bMadeSelection;
}

bool NNetWindow::OnRButtonDown( WPARAM const wParam, LPARAM const lParam )
{
	SetFocus( );
	return false;
}

void NNetWindow::OnSetCursor( WPARAM const wParam, LPARAM const lParam )
{
	bool    const keyDown = GetAsyncKeyState(VK_LBUTTON) & 0x8000;
	HCURSOR const hCrsr   = keyDown ? m_hCrsrMove : m_hCrsrArrow;
	SetCursor( hCrsr );
}

bool NNetWindow::OnCommand( WPARAM const wParam, LPARAM const lParam, PixelPoint const pixPoint )
{
	MicroMeterPoint const umPoint { m_context.GetCoordC().Convert2MicroMeterPointPos( pixPoint ) };
	if ( m_pController->HandleCommand( LOWORD( wParam ), lParam, umPoint ) )
		return true;
	else 
		return ModelWindow::OnCommand( wParam, lParam, pixPoint );
}
