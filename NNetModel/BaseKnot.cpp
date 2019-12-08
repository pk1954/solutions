// BaseKnot.cpp
//
// NNetModel

#include "stdafx.h"
#include "assert.h"
#include "Geometry.h"
#include "NNetModel.h"
#include "PixelCoordsFp.h"
#include "win32_graphicsInterface.h"
#include "BaseKnot.h"

using std::find;
using std::begin;
using std::end;

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
