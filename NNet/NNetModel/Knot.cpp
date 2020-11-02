// Knot.cpp 
//
// NNetModel

#include "stdafx.h"
#include <assert.h>
#include "tHighlightType.h"
#include "DrawContext.h"
#include "Knot.h"

Knot::Knot( Knot const & src ) :  // copy constructor
	BaseKnot( src )
{ }

void Knot::DrawExterior( DrawContext const & context, tHighlightType const type ) const
{
	MicroMeter const umRadius { (type == tHighlightType::normal) ? GetExtension( ) : 30.0_MicroMeter };
	context.DrawCircle( MicroMeterCircle( GetPosition(), umRadius ), GetFrameColor( type )	);
}

void Knot::DrawInterior( DrawContext const & context ) const
{
	context.DrawCircle( GetCircle() * PIPE_INTERIOR, GetInteriorColor( ) );
}

void Knot::CheckShape( ) const
{
	BaseKnot::CheckShape();
	assert( ! m_connections.IsOrphan() );
}
