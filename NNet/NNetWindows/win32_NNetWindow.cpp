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

NNetWindow::NNetWindow() :
	ModelWindow( )
{ }

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
	m_graphics.Initialize( hwnd );
	m_context.Start( & m_graphics );
	m_pNMRI           = & modelReaderInterface;
	m_pMonitorWindow  = & monitorWindow;
	m_pController     = & controller;
	m_fPixRadiusLimit = fPixLimit;
	
	//m_upBeaconAnimation = make_unique<Animation<float>>(nullptr, ANIMATION_RECURRING);
	//m_upBeaconAnimation->Start( 0.0f, 1.0f );
	ShowRefreshRateDlg( bShowRefreshRateDialog );
}

void NNetWindow::Stop( )
{
	m_context.Stop();
	DestroyWindow( );
}

NNetWindow::~NNetWindow( )
{
	m_pNMRI       = nullptr;
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
	MicroMeterRect const umRect { GetCoordC().Transform2MicroMeterRect(rect) };
	m_pNMRI->GetUPShapes().Apply2AllInRect<Shape>
	(
		umRect,
		[&](Shape const & s) 
		{ 
			if (crit(s)) 
				s.DrawInterior( m_context, tHighlight::normal ); 
		} 
	);
}

void NNetWindow::DrawExteriorInRect( PixelRect const & rect ) const
{
	MicroMeterRect const umRect { GetCoordC().Transform2MicroMeterRect(rect) };
	m_pNMRI->GetUPShapes().Apply2AllInRect<Shape>
	( 
		umRect,
		[&](Shape const & s) 
		{ 
			s.DrawExterior( m_context, tHighlight::normal ); 
		} 
	);
}

void NNetWindow::DrawArrowsInRect( PixelRect const & rect, MicroMeter const umSize ) const
{
	MicroMeterRect const umRect { GetCoordC().Transform2MicroMeterRect(rect) };
	m_pNMRI->GetUPShapes().Apply2AllInRect<Pipe>
	( 
		umRect,	
		[&](Pipe const & s) { s.DrawArrows(m_context, umSize); } 
	);
}

void NNetWindow::DrawNeuronTextInRect( PixelRect const & rect ) const
{
	MicroMeterRect const umRect { GetCoordC().Transform2MicroMeterRect(rect) };
	if ( PixelSize() <= 2.5_MicroMeter )
	{
		m_pNMRI->GetUPShapes().Apply2AllInRect<Neuron>
		( 
			umRect,
			[&](Neuron const & n) { n.DrawNeuronText( m_context ); } 
		);
	}
}

void NNetWindow::DrawSensors( ) const
{
	m_pNMRI->GetMonitorData().Apply2AllSignals
	(
		[&](Signal const & signal) { signal.Draw( m_context ); }
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
	//MicroMeterCircle umCircleBeacon { m_pMonitorWindow->GetSelectedSignalCircle() };
	//if ( umCircleBeacon.IsNotNull() )
	//{
	//	MicroMeter const umRadiusLimit  { GetCoordC().Transform2MicroMeter( m_fPixRadiusLimit ) };
	//	MicroMeter const umMaxRadius    { max( umCircleBeacon.GetRadius(), umRadiusLimit ) };
	//	MicroMeter const umSpan         { umMaxRadius - NEURON_RADIUS };
	//	float            fRelBeaconSize = m_upBeaconAnimation->GetActual();
	//	umCircleBeacon.SetRadius( NEURON_RADIUS + (umSpan * fRelBeaconSize)  );
	//	m_context.FillCircle
	//	( 
	//		umCircleBeacon, 
	//		NNetColors::SetAlpha( NNetColors::COL_BEACON, 0.8f * (1.0f - fRelBeaconSize) )
	//	);
	//}
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
