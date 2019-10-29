// BaseKnot.cpp
//
// NNetModel

#include "stdafx.h"
#include "Geometry.h"
#include "NNetModel.h"
#include "win32_graphicsInterface.h"
#include "BaseKnot.h"

void BaseKnot::AddIncomming( ShapeId const idPipeline )
{
	m_incoming.push_back( idPipeline );
}

void BaseKnot::AddOutgoing( ShapeId const idPipeline )
{
	m_outgoing.push_back( idPipeline );
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
		m_pModel->GetPipeline( idPipeline )->Resize( );

	for ( auto const idPipeline : m_outgoing )
		m_pModel->GetPipeline( idPipeline )->Resize( );
}

void BaseKnot::drawPolygon
(
	int        const iNrOfEdges,
	COLORREF   const color, 
	MicroMeter const umWidth
) const
{
	m_pGraphics->DrawPolygon
	( 
		iNrOfEdges,
		m_pCoord->convert2fPixelPos( GetPosition() ), 
		color, 
		m_pCoord->convert2fPixel( umWidth )
	);
}
