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
#include "BeaconAnimation.h"
#include "NNetColors.h"
#include "MonitorData.h"
#include "win32_sound.h"
#include "win32_tooltip.h"
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
	BeaconAnimation          * const pBeaconAnimation,
	MonitorData              * const pMonitorData
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
	m_pController      = pController;
	m_pMRI             = pModelReaderInterface;
	m_pBeaconAnimation = pBeaconAnimation;
	m_pMonitorData     = pMonitorData;

	ShowRefreshRateDlg( bShowRefreshRateDialog );
}

void NNetWindow::Stop( )
{
	m_context.Stop();
	DestroyWindow( );
}

NNetWindow::~NNetWindow( )
{
	m_pMRI        = nullptr;
	m_pController = nullptr;
}

MicroMeterRect const NNetWindow::GetEnclosingRect() const 
{ 
	return m_pMRI->GetEnclosingRect(); 
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
	m_pMRI->Apply2AllInRect<Shape>
	(
		GetCoord().Convert2MicroMeterRect( rect ),
		[&](Shape const & s) { if (crit(s)) s.DrawInterior( m_context ); } 
	);
}

void NNetWindow::DrawExteriorInRect( PixelRect const & rect ) const
{
	MicroMeterRect umRect { GetCoord().Convert2MicroMeterRect( rect ) }; 
	m_pMRI->Apply2AllInRect<Shape>
	( 
		GetCoord().Convert2MicroMeterRect( rect ),	
		[&](Shape const & s) { s.DrawExterior( m_context ); } 
	);
}

void NNetWindow::DrawNeuronTextInRect( PixelRect const & rect ) const
{
	m_pMRI->Apply2AllInRect<Neuron>
	( 
		GetCoord().Convert2MicroMeterRect( rect ),
		[&](Neuron const & n) { n.DrawNeuronText( m_context ); } 
	);
}

void NNetWindow::DrawSensors( ) const
{
	m_pMonitorData->Apply2AllSignals
	(
		[&](SignalInterface const & signal)
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
	return m_pMRI->FindShapeAt
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

void NNetWindow::AnimateBeaconSingle( fPIXEL const fPixBeaconRadius )
{
	ShapeId idBeacon { m_pBeaconAnimation->GetBeaconShapeId() };
	if ( IsDefined( idBeacon ) )
	{
		assert( m_pMRI->IsValidShapeId(idBeacon) );

		static MicroMeter const MIN_SIZE { NEURON_RADIUS };
		static MicroMeter const MAX_SIZE { NEURON_RADIUS * 2 };

		MicroMeter        const umMaxSize{ max( MAX_SIZE, GetCoord().Convert2MicroMeter( fPixBeaconRadius ) ) };
		MicroMeter        const umSpan   { umMaxSize - MIN_SIZE };
		float             const fRelSize { static_cast<float>(m_pBeaconAnimation->GetPercentage().GetValue()) / 100.0f };
		MicroMeter        const umRadius { MIN_SIZE + (umSpan * fRelSize)  };
		MicroMeterPoint   const umPos    { m_pMRI->GetShapePos( idBeacon ) };
		if ( umPos.IsNotNull() )
		{
			MicroMeterCircle  const umCircle { umPos, umRadius };
			D2D1::ColorF col { NNetColors::COL_BEACON };
			col.a = 1.0f - fRelSize;
			m_context.FillCircle( umCircle, col );
		}
	}
}

void NNetWindow::AnimateBeacon( fPIXEL const fPixBeaconRadius )
{
	MicroMeterCircle const & circle { m_pBeaconAnimation->GetSensorCircle( ) };
	if ( circle.IsNotNull() )
	{
		static MicroMeter const MIN_SIZE { NEURON_RADIUS };
		static MicroMeter const MAX_SIZE { circle.GetRadius() };

		MicroMeter        const umMaxSize{ max( MAX_SIZE, GetCoord().Convert2MicroMeter( fPixBeaconRadius ) ) };
		MicroMeter        const umSpan   { umMaxSize - MIN_SIZE };
		float             const fRelSize { static_cast<float>(m_pBeaconAnimation->GetPercentage().GetValue()) / 100.0f };
		MicroMeter        const umRadius { MIN_SIZE + (umSpan * fRelSize)  };
		MicroMeterPoint   const umPos    { circle.GetPosition() };
		if ( umPos.IsNotNull() )
		{
			MicroMeterCircle  const umCircle { umPos, umRadius };
			D2D1::ColorF col { NNetColors::COL_BEACON };
			col.a = 1.0f - fRelSize;
			m_context.FillCircle( umCircle, col );
		}
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
