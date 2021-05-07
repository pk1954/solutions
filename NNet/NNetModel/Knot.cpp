// Knot.cpp 
//
// NNetModel

#include "stdafx.h"
#include <assert.h>
#include "tHighlightType.h"
#include "DrawContext.h"
#include "Knot.h"

void Knot::DrawExterior( DrawContext const & context, tHighlight const type ) const
{
	MicroMeter const umRadius 
	{ 
		(type == tHighlight::highlighted) 
		? 30.0_MicroMeter 
		: GetExtension() 
	};
	context.FillCircle(MicroMeterCircle(GetPos(), umRadius), GetExteriorColor(type));
}

void Knot::DrawInterior( DrawContext const & context, tHighlight const type ) const
{
	context.FillCircle( GetCircle() * PIPE_INTERIOR, GetInteriorColor(type) );
}

void Knot::Check() const
{
	BaseKnot::Check();
	if ( m_connections.IsOrphan() )
	{
		int x = 42;
	}
	assert( ! m_connections.IsOrphan() );
}

void Knot::Select(bool const bOn, bool const bRecursive) 
{ 
	if (bRecursive)
		m_connections.Apply2AllConnectedPipes([&](Pipe &p){ p.Select(bOn, true); });

	bool bAnyConnectedPipeSelected = m_connections.Apply2AllConnectedPipesB
	( 
		[&](Pipe const &p) { return p.IsSelected(); }   // if any connected pipe is selected
	);                                                  // knot must also be selected

	Nob::Select( bOn || bAnyConnectedPipeSelected );
}
