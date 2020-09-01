// win32_NNetWindow.cpp
//
// NNetWindows

#include "stdafx.h"
#include <sstream> 
#include "MoreTypes.h"
#include "Segment.h"
#include "scale.h"
#include "Pipe.h"
#include "InputNeuron.h"
#include "PixelTypes.h"
#include "NNetParameters.h"
#include "NNetModelReaderInterface.h"
#include "NNetColors.h"
#include "NNetModel.h"
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
	NNetModelReaderInterface * const pModelReaderInterface
)
{
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
	ShowRefreshRateDlg( bShowRefreshRateDialog );
}

void NNetWindow::Stop( )
{
	m_context.Stop();
	DestroyWindow( );
}

NNetWindow::~NNetWindow( )
{
	m_pModelReaderInterface = nullptr;
	m_pController           = nullptr;
}

MicroMeterRect const NNetWindow::GetEnclosingRect() const 
{ 
	return m_pModelReaderInterface->GetEnclosingRect(); 
}

MicroMeterRect const NNetWindow::GetViewRect() const 
{ 
	return GetCoord().Convert2MicroMeterRect( GetClPixelRect() ); 
};

void NNetWindow::DrawInteriorInRect
( 
	PixelRect   const & rect, 
	ShapeCrit   const & crit 
) const
{
	MicroMeterRect umRect { GetCoord().Convert2MicroMeterRect( rect ) }; 
	m_pModelReaderInterface->Apply2AllInRect<Shape>
	(
		GetCoord().Convert2MicroMeterRect( rect ),
		[&](Shape const & s) { if (crit(s)) s.DrawInterior( m_context ); } 
	);
}

void NNetWindow::DrawExteriorInRect( PixelRect const & rect ) const
{
	MicroMeterRect umRect { GetCoord().Convert2MicroMeterRect( rect ) }; 
	m_pModelReaderInterface->Apply2AllInRect<Shape>
	( 
		GetCoord().Convert2MicroMeterRect( rect ),	
		[&](Shape const & s) { s.DrawExterior( m_context ); } 
	);
}

void NNetWindow::DrawNeuronTextInRect( PixelRect const & rect ) const
{
	m_pModelReaderInterface->Apply2AllInRect<Neuron>
	( 
		GetCoord().Convert2MicroMeterRect( rect ),
		[&](Neuron const & n) { n.DrawNeuronText( m_context ); } 
	);
}

ShapeId const NNetWindow::FindShapeAt
( 
	PixelPoint const & pixPoint, 
	ShapeCrit  const & crit 
) const
{	
	return m_pModelReaderInterface->FindShapeAt
	( 
		GetCoord().Convert2MicroMeterPointPos( pixPoint ), 
		[&]( Shape const & s ) { return crit( s ); } 
	);
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
}

bool NNetWindow::OnSize( WPARAM const wParam, LPARAM const lParam )
{
	UINT width  = LOWORD(lParam);
	UINT height = HIWORD(lParam);
	m_context.Resize( width, height );
	Notify( false );
	return true;    // job done
}

bool NNetWindow::OnCommand( WPARAM const wParam, LPARAM const lParam, PixelPoint const pixPoint )
{
	MicroMeterPoint const umPoint { GetCoord().Convert2MicroMeterPointPos( pixPoint ) };
	if ( m_pController->HandleCommand( LOWORD( wParam ), lParam, umPoint ) )
		return true;

	return ModelWindow::OnCommand( wParam, lParam, pixPoint );
}
