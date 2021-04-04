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
	MicroMeter const umRadius 
	{ 
		(type == tHighlightType::highlighted) 
		? 30.0_MicroMeter 
		: GetExtension() 
	};
	context.FillCircle(MicroMeterCircle(GetPosition(), umRadius), GetExteriorColor(type));
}

void Knot::DrawInterior( DrawContext const & context, tHighlightType const type ) const
{
	context.FillCircle( GetCircle() * PIPE_INTERIOR, GetInteriorColor(type) );
}

void Knot::CheckShape() const
{
	BaseKnot::CheckShape();
	if ( m_connections.IsOrphan( ) )
	{
		int x = 42;
	}
	assert( ! m_connections.IsOrphan() );
}

void Knot::Select(bool const bOn, bool const bRecursive) 
{ 
	Shape::Select(bOn, false);
	if (bRecursive)
		m_connections.Apply2AllConnectedPipes([&](Pipe &p){ p.Select(bOn, false); });
}
