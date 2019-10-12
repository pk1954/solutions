// BaseKnot.cpp
//
// NNetModel

#include "stdafx.h"
#include "Geometry.h"
#include "NNetModel.h"
#include "BaseKnot.h"

void BaseKnot::AddIncomming( ShapeId const idPipeline )
{
	m_incomming.push_back( idPipeline );
}

void BaseKnot::AddOutgoing( ShapeId const idPipeline )
{
	m_outgoing.push_back( idPipeline );
}

bool BaseKnot::IsPointInShape( NNetModel const & model, MicroMeterPoint const & point ) const
{
	MicroMeterPoint const corner1 = m_center + MicroMeterPoint( + m_extension, + m_extension );
	MicroMeterPoint const corner2 = m_center + MicroMeterPoint( + m_extension, - m_extension );
	MicroMeterPoint const corner3 = m_center + MicroMeterPoint( - m_extension, + m_extension );

	return IsPointInRect< MicroMeterPoint >( point, corner1, corner2, corner3 );
}

void BaseKnot::MoveTo( NNetModel & model, MicroMeterPoint const & newCenter )
{
	m_center = newCenter;
	for ( auto const idPipeline : m_incomming )
		model.GetPipeline( idPipeline )->Resize( model );

	for ( auto const idPipeline : m_outgoing )
		model.GetPipeline( idPipeline )->Resize( model );
}
