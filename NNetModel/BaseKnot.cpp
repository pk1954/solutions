// BaseKnot.cpp
//
// NNetModel

#include "stdafx.h"
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
	m_incoming.push_back( idPipeline );
}

void BaseKnot::AddOutgoing( ShapeId const idPipeline )
{
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

bool BaseKnot::IsPointInShape
( 
	NNetModel       const & model, 
	MicroMeterPoint const & point 
) const
{
	MicroMeterPoint const corner1 = m_center + MicroMeterPoint( + m_extension, + m_extension );
	MicroMeterPoint const corner2 = m_center + MicroMeterPoint( + m_extension, - m_extension );
	MicroMeterPoint const corner3 = m_center + MicroMeterPoint( - m_extension, + m_extension );

	return IsPointInRect< MicroMeterPoint >( point, corner1, corner2, corner3 );
}

void BaseKnot::MoveTo
( 
	NNetModel             & model,
	MicroMeterPoint const & newCenter 
)
{
	m_center = newCenter;
	for ( auto const idPipeline : m_incoming )
		model.GetPipeline( idPipeline )->Recalc( model );

	for ( auto const idPipeline : m_outgoing )
		model.GetPipeline( idPipeline )->Recalc( model );
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
