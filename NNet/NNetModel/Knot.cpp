// Knot.cpp 
//
// NNetModel

#include "stdafx.h"
#include <assert.h>
#include "tHighlightType.h"
#include "DrawContext.h"
#include "Knot.h"

void Knot::DrawExterior( DrawContext const & context, tHighlightType const type ) const
{
	MicroMeter const umRadius { (type == tHighlightType::normal) ? GetExtension( ) : 30.0_MicroMeter };
	context.FillCircle( MicroMeterCircle( GetPosition(), umRadius ), GetFrameColor( type )	);
}

void Knot::DrawInterior( DrawContext const & context ) const
{
	context.FillCircle( GetCircle() * PIPE_INTERIOR, GetInteriorColor( ) );
}

void Knot::CheckShape( ) const
{
	BaseKnot::CheckShape();
	if ( m_connections.IsOrphan( ) )
	{
		int x = 42;
	}
	assert( ! m_connections.IsOrphan() );
}
