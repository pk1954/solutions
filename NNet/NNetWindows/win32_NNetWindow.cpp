// win32_NNetWindow.cpp
//
// NNetWindows

#include "stdafx.h"
#include <sstream> 
#include "util.h"
#include "MoreTypes.h"
#include "Segment.h"
#include "scale.h"
#include "Pipe.h"
#include "InputNeuron.h"
#include "PixelTypes.h"
#include "NNetParameters.h"
#include "NNetModelReaderInterface.h"
#include "NNetModelWriterInterface.h"
#include "NNetColors.h"
#include "MonitorData.h"
#include "win32_sound.h"
#include "win32_monitorWindow.h"
#include "win32_NNetWindow.h"

using std::function;

void NNetWindow::InitClass( ActionTimer * const pActionTimer )
{
	ModelWindow::InitClass( pActionTimer );
}

NNetWindow::NNetWindow( ) :
	ModelWindow( )
{ }

void TimerprocBeacon( HWND hwnd, UINT msgTimer, UINT_PTR idTimer, DWORD msSinceStart )
{
	NNetWindow * pNNetWin { reinterpret_cast<NNetWindow *>(GetUserDataPtr( hwnd )) };
	if ( pNNetWin->m_beaconAnimation.Next( false ) )
		pNNetWin->m_fRelBeaconSize = 0.0f;
	pNNetWin->Notify( false );
}

void NNetWindow::Start
( 
	HWND                     const   hwndApp, 
	DWORD                    const   dwStyle,
	bool                     const   bShowRefreshRateDialog,
	fPixel                   const   fPixLimit,
	NNetModelReaderInterface const & modelReaderInterface,
	MonitorWindow            const & monitorWindow,
	NNetController                 & controller
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
	m_pNMRI           = & modelReaderInterface;
	m_pMonitorWindow  = & monitorWindow;
	m_pController     = & controller;
	m_fPixRadiusLimit = fPixLimit;
	m_beaconAnimation.Start(GetWindowHandle(), m_fRelBeaconSize, 1.0f, ID_BEACON_TIMER, TimerprocBeacon);
	ShowRefreshRateDlg( bShowRefreshRateDialog );
}

void NNetWindow::Stop( )
{
	m_context.Stop();
	DestroyWindow( );
}

NNetWindow::~NNetWindow( )
{
	m_pNMRI        = nullptr;
	m_pController = nullptr;
}

MicroMeterRect const NNetWindow::GetViewRect() const 
{ 
	return GetCoordC().Transform2MicroMeterRect( GetClPixelRect() ); 
};

void NNetWindow::DrawInteriorInRect
( 
	PixelRect const & rect, 
	ShapeCrit const & crit 
) const
{
	m_pNMRI->GetShapes().Apply2AllInRect<Shape>
	(
		GetCoordC().Transform2MicroMeterRect( rect ),
		[&](Shape const & s) 
		{ 
			if (crit(s)) 
				s.DrawInterior( m_context, tHighlightType::normal ); 
		} 
	);
}

void NNetWindow::DrawExteriorInRect( PixelRect const & rect ) const
{
	m_pNMRI->GetShapes().Apply2AllInRect<Shape>
	( 
		GetCoordC().Transform2MicroMeterRect( rect ),	
		[&](Shape const & s) 
		{ 
			s.DrawExterior( m_context, tHighlightType::normal ); 
		} 
	);
}

void NNetWindow::DrawArrowsInRect( PixelRect const & rect, MicroMeter const umSize ) const
{
	m_pNMRI->GetShapes().Apply2AllInRect<Pipe>
	( 
		GetCoordC().Transform2MicroMeterRect( rect ),	
		[&](Pipe const & s) 
		{ 
			s.DrawArrows( m_context, tHighlightType::normal, umSize ); 
		} 
	);
}

void NNetWindow::DrawNeuronTextInRect( PixelRect const & rect ) const
{
	if ( PixelSize() <= 2.5_MicroMeter )
	{
		m_pNMRI->GetShapes().Apply2AllInRect<Neuron>
		( 
			GetCoordC().Transform2MicroMeterRect( rect ),
			[&](Neuron const & n) { n.DrawNeuronText( m_context ); } 
		);
	}
}

void NNetWindow::DrawSensors( ) const
{
	m_pNMRI->GetMonitorData().Apply2AllSignals
	(
		[&](Signal const & signal)
		{
			signal.Draw( m_context );
		}
	);
}

ShapeId const NNetWindow::FindShapeAt
( 
	PixelPoint const & pixPoint, 
	ShapeCrit  const & crit 
) const
{	
	return m_pNMRI->FindShapeAt
	( 
		GetCoordC().Transform2MicroMeterPointPos( pixPoint ), 
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

void NNetWindow::DrawBeacon( )
{
	MicroMeterCircle umCircleBeacon { m_pMonitorWindow->GetSelectedSignalCircle() };
	if ( umCircleBeacon.IsNotNull() )
	{
		MicroMeter const umRadiusLimit { GetCoordC().Transform2MicroMeter( m_fPixRadiusLimit ) };
		MicroMeter const umMaxRadius   { max( umCircleBeacon.GetRadius(), umRadiusLimit ) };
		MicroMeter const umSpan        { umMaxRadius - NEURON_RADIUS };
		umCircleBeacon.SetRadius( NEURON_RADIUS + (umSpan * m_fRelBeaconSize)  );
		m_context.FillCircle
		( 
			umCircleBeacon, 
			NNetColors::SetAlpha( NNetColors::COL_BEACON, 0.8f * (1.0f - m_fRelBeaconSize) )
		);
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
	MicroMeterPoint const umPoint { GetCoordC().Transform2MicroMeterPointPos( pixPoint ) };
	if ( m_pController->HandleCommand( LOWORD( wParam ), lParam, umPoint ) )
		return true;

	return ModelWindow::OnCommand( wParam, lParam, pixPoint );
}

//bool NNetWindow::OnTimer( WPARAM const wParam, LPARAM const lParam )
//{
//	switch (wParam)
//	{
//	case ID_BEACON_TIMER:
//		if ( m_beaconAnimation.Next( false ) )
//			m_fRelBeaconSize = 0.0f;
//		Notify( false );
//		break;
//	}
//	return ModelWindow::OnTimer( wParam, lParam );
//}

