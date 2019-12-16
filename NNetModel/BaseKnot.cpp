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

ShapeId BaseKnot::GetPrecursor( ) const
{
	assert( m_incoming.size() == 1 );
	Pipeline const * const pIncoming { m_pNNetModel->GetTypedShape<Pipeline>( m_incoming[0] ) };
	return pIncoming->GetStartKnot( );
}

ShapeId BaseKnot::GetSuccessor( ) const
{
	assert( m_outgoing.size() == 1 );
	Pipeline const * const pOutgoing { m_pNNetModel->GetTypedShape<Pipeline>( m_outgoing[0] ) };
	return pOutgoing->GetEndKnot( );
}

void BaseKnot::AddIncomming( ShapeId const idPipeline )
{
	assert( find( begin(m_incoming), end(m_incoming), idPipeline ) == end(m_incoming) );
	m_incoming.push_back( idPipeline );
}

void BaseKnot::AddOutgoing( ShapeId const idPipeline )
{
	assert( find( begin(m_outgoing), end(m_outgoing), idPipeline ) == end(m_outgoing) );
	m_outgoing.push_back( idPipeline );
}

void BaseKnot::RemoveIncoming( ShapeId const idPipeline )
{
	auto res = find( begin(m_incoming), end(m_incoming), idPipeline );
	m_incoming.erase( res );
}

void BaseKnot::RemoveOutgoing( ShapeId const idPipeline )
{
	auto res = find( begin(m_outgoing), end(m_outgoing), idPipeline );
	m_outgoing.erase( res );
}

void BaseKnot::ReplaceIncoming( ShapeId const idPipelineOld, ShapeId const idPipelineNew )
{
	assert( find( begin(m_incoming), end(m_incoming), idPipelineNew ) == end(m_incoming) );
	* find( begin(m_incoming), end(m_incoming), idPipelineOld ) = idPipelineNew;
}

void BaseKnot::ReplaceOutgoing( ShapeId const idPipelineOld, ShapeId const idPipelineNew )
{
	assert( find( begin(m_outgoing), end(m_outgoing), idPipelineNew ) == end(m_outgoing) );
	* find( begin(m_outgoing), end(m_outgoing), idPipelineOld ) = idPipelineNew;
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
		pixPos.GetX() - pixExt,  // left
		pixPos.GetY() - pixExt,  // top
		pixPos.GetX() + pixExt,  // right
		pixPos.GetY() + pixExt   // bottom
	};
	return pixRect;
}

bool const BaseKnot::DisplayText( PixelRect const pixRect, wstring const text ) const
{
	static COLORREF const color { RGB( 0, 255, 0 ) };

	bool bTextFits { pixRect.Includes( m_pGraphics->CalcGraphicsRect( text ).GetSize() ) };
	
	if ( bTextFits )
		m_pGraphics->DisplayGraphicsText( pixRect, text, DT_CENTER|DT_VCENTER, color );

	return bTextFits;
}

void BaseKnot::DrawText( PixelCoordsFp & coord ) const
{ 
	wostringstream m_wBuffer;
	m_wBuffer.precision(2);

	float fFillLevel { GetFillLevel() };
	if ( fFillLevel >= 1.0f )
		m_wBuffer << L"TRIGGER";
	else
		m_wBuffer << setw(2) << GetFillLevel() * 100 << L"%";

	PixelRect const pixRect { GetPixRect4Text( coord ) };

	DisplayText( pixRect, m_wBuffer.str( ) );
}

void BaseKnot::MoveTo( MicroMeterPoint const & newCenter )
{
	m_center = newCenter;
	for ( auto const idPipeline : m_incoming )
		m_pNNetModel->GetTypedShape<Pipeline>( idPipeline )->Recalc( );

	for ( auto const idPipeline : m_outgoing )
		m_pNNetModel->GetTypedShape<Pipeline>( idPipeline )->Recalc( );
}

void BaseKnot::drawPolygon
(
	PixelCoordsFp   const & coord,
	int             const   iNrOfEdges,
	COLORREF        const   color, 
	MicroMeterPoint const   umCenter,
	MicroMeter      const   umWidth
) const
{
	m_pGraphics->DrawPolygon
	( 
		iNrOfEdges,
		coord.convert2fPixelPos( umCenter ), 
		color, 
		coord.convert2fPixel( umWidth )
	);
}

void BaseKnot::drawPolygon
(
	PixelCoordsFp const & coord,
	int           const   iNrOfEdges,
	COLORREF      const   color, 
	MicroMeter    const   umWidth
) const
{
	drawPolygon( coord, iNrOfEdges, color, GetPosition(), umWidth );
}
