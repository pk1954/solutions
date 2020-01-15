// BaseKnot.cpp
//
// NNetModel

#include "stdafx.h"
#include "assert.h"
#include <iomanip>
#include "Geometry.h"
#include "NNetModel.h"
#include "PixelCoordsFp.h"
#include "Direct2D.h"
#include "BaseKnot.h"

using std::find;
using std::begin;
using std::end;
using std::wstring;
using std::wostringstream;

bool BaseKnot::IsPrecursorOf( ShapeId const id )
{
	bool bConnectionFound { false };

	Apply2AllOutgoingPipelines
	( 
		[&]( Pipeline const * const pipe ) 
		{
			if ( pipe->GetEndKnotId( ) == id ) 
				bConnectionFound = true; 
		} 
	);

	return bConnectionFound;
}

bool BaseKnot::IsSuccessorOf( ShapeId const id )
{
	bool bConnectionFound { false };

	Apply2AllIncomingPipelines
	( 
		[&]( Pipeline const * const pipe ) 
		{
			if ( pipe->GetStartKnotId( ) == id ) 
				bConnectionFound = true; 
		} 
	);

	return bConnectionFound;
}

void BaseKnot::AddIncoming( Pipeline * const pPipeline )
{
	assert( find( begin(m_incoming), end(m_incoming), pPipeline ) == end(m_incoming) );
	m_incoming.push_back( pPipeline );
}

void BaseKnot::AddOutgoing( Pipeline * const pPipeline )
{
	assert( find( begin(m_outgoing), end(m_outgoing), pPipeline ) == end(m_outgoing) );
	m_outgoing.push_back( pPipeline );
}

void BaseKnot::RemoveIncoming( Pipeline * const pPipeline )
{
	auto res = find( begin(m_incoming), end(m_incoming), pPipeline );
	m_incoming.erase( res );
}

void BaseKnot::RemoveOutgoing( Pipeline * const pPipeline )
{
	auto res = find( begin(m_outgoing), end(m_outgoing), pPipeline );
	m_outgoing.erase( res );
}

void BaseKnot::ReplaceIncoming( Pipeline * const pPipelineOld, Pipeline * const pPipelineNew )
{
	assert( find( begin(m_incoming), end(m_incoming), pPipelineNew ) == end(m_incoming) );
	* find( begin(m_incoming), end(m_incoming), pPipelineOld ) = pPipelineNew;
}

void BaseKnot::ReplaceOutgoing( Pipeline * const pPipelineOld, Pipeline * const pPipelineNew )
{
	assert( find( begin(m_outgoing), end(m_outgoing), pPipelineNew ) == end(m_outgoing) );
	* find( begin(m_outgoing), end(m_outgoing), pPipelineOld ) = pPipelineNew;
}

bool BaseKnot::IsPointInShape( MicroMeterPoint const & point ) const
{
	MicroMeterPoint const corner1 = m_center + MicroMeterPoint( + m_extension, + m_extension );
	MicroMeterPoint const corner2 = m_center + MicroMeterPoint( + m_extension, - m_extension );
	MicroMeterPoint const corner3 = m_center + MicroMeterPoint( - m_extension, + m_extension );

	return IsPointInRect< MicroMeterPoint >( point, corner1, corner2, corner3 );
}

PixelRect const BaseKnot::GetPixRect4Text( PixelCoordsFp const & coord ) const
{
	fPixelPoint const fPos   { coord.convert2fPixelPos( GetPosition() ) }; 
	fPIXEL      const fExt   { coord.convert2fPixel( GetExtension() ) };
	PixelPoint  const pixPos { convert2PixelPoint( fPos ) };
	PIXEL       const pixExt { PIXEL(static_cast<long>(fExt.GetValue())) };
	PixelRect   const pixRect
	{
		pixPos.GetX() - pixExt,      // left
		pixPos.GetY() - pixExt / 2,  // top
		pixPos.GetX() + pixExt,      // right
		pixPos.GetY() + pixExt       // bottom
	};
	return pixRect;
}

void const BaseKnot::DisplayText( PixelRect const pixRect, wstring const text ) const
{
	static COLORREF const color { RGB( 0, 255, 0 ) };

	m_pGraphics->DisplayText( pixRect, text, color );
}

void BaseKnot::DrawNeuronText( PixelCoordsFp & coord ) const
{ 
	wostringstream m_wBuffer;
	m_wBuffer.precision(2);

	float fFillLevel { GetFillLevel() };
//	m_wBuffer << std::endl;
	if ( fFillLevel >= 1.0f )
		m_wBuffer << L"TRIGGER";
	else
		m_wBuffer << setw(5) << GetFillLevel() * 100 << L"%";

	PixelRect const pixRect { GetPixRect4Text( coord ) };

	DisplayText( pixRect, m_wBuffer.str( ) );
}

void BaseKnot::MoveShape( MicroMeterPoint const & delta )
{
	m_center += delta;
	for ( auto const pipe : m_incoming )
		pipe->Recalc( );

	for ( auto const pipe : m_outgoing )
		pipe->Recalc( );
}

void BaseKnot::drawCircle
(
	PixelCoordsFp   const & coord,
	COLORREF        const   color, 
	MicroMeterPoint const   umCenter,
	MicroMeter      const   umWidth
) const
{
	m_pGraphics->DrawCircle
	( 
		coord.convert2fPixelPos( umCenter ), 
		color, 
		coord.convert2fPixel( umWidth )
	);
}

void BaseKnot::drawCircle
(
	PixelCoordsFp const & coord,
	COLORREF      const   color, 
	MicroMeter    const   umWidth
) const
{
	m_pGraphics->DrawCircle
	( 
		coord.convert2fPixelPos( GetPosition() ), 
		color, 
		coord.convert2fPixel( umWidth )
	);
}
