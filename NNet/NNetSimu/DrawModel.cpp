// DrawModel.cpp
//
// NNetSimu

#include "stdafx.h"
#include "NNetModel.h"
#include "DrawModel.h"

void DrawModel::DrawInteriorInRect
( 
	PixelRect   const & rect, 
	DrawContext const & context, 
	ShapeCrit   const & crit 
) const
{
	MicroMeterRect umRect { context.GetCoordC().Convert2MicroMeterRect( rect ) }; 
	m_pModel->Apply2All<Shape>
	( 
		[&](Shape const & s) { if (crit(s) && s.IsInRect(umRect)) s.DrawInterior( context ); } 
	);
}

void DrawModel::DrawExteriorInRect
( 
	PixelRect   const & rect, 
	DrawContext const & context 
) const
{
	MicroMeterRect umRect { context.GetCoordC().Convert2MicroMeterRect( rect ) }; 
	m_pModel->Apply2All<Shape>
	( 
		[&](Shape const & s) { if (s.IsInRect(umRect)) s.DrawExterior( context ); } 
	);
}

void DrawModel::DrawNeuronTextInRect
( 
	PixelRect   const & rect, 
	DrawContext const & context 
) const
{
	MicroMeterRect umRect { context.GetCoordC().Convert2MicroMeterRect( rect ) }; 
	m_pModel->Apply2All<Neuron>
	( 
		[&](Neuron const & n) { if (n.IsInRect(umRect)) n.DrawNeuronText( context ); } 
	);
}

ShapeId const DrawModel::FindShapeAt
( 
	PixelPoint  const & pixPoint, 
	DrawContext const & context, 
	ShapeCrit   const & crit 
) const
{	
	MicroMeterPoint umPoint { context.GetCoordC().Convert2MicroMeterPointPos( pixPoint ) };
	ShapeId idRes { NO_SHAPE };

	if ( idRes == NO_SHAPE )   // first test all neurons and input neurons
		idRes = m_pModel->FindShapeAt( umPoint, [&]( Shape const & s ) { return s.IsAnyNeuron( ) && crit( s ); } );

	if ( idRes == NO_SHAPE )   // if nothing found, test knot shapes
		idRes = m_pModel->FindShapeAt( umPoint, [&]( Shape const & s ) { return s.IsKnot     ( ) && crit( s ); } ); 	

	if ( idRes == NO_SHAPE )   // if nothing found, try pipes
		idRes = m_pModel->FindShapeAt( umPoint, [&]( Shape const & s ) { return s.IsPipe     ( ) && crit( s ); } );

	return idRes;
}
