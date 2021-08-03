// Knot.cpp 
//
// NNetModel

#include "stdafx.h"
#include <assert.h>
#include "tHighlightType.h"
#include "DrawContext.h"
#include "Knot.h"

void Knot::DrawExterior(DrawContext const & context, tHighlight const type) const
{
	MicroMeter const umRadius 
	{ 
		(type == tHighlight::highlighted) 
		? 30.0_MicroMeter 
		: GetExtension() 
	};
	context.FillCircle(MicroMeterCircle(GetPos(), umRadius), GetExteriorColor(type));
}

void Knot::DrawInterior(DrawContext const & context, tHighlight const type) const
{
	context.FillCircle(GetCircle() * PIPE_INTERIOR, GetInteriorColor(type));
}

void Knot::Check() const
{
	BaseKnot::Check();
	assert(! IsOrphan());
}

void Knot::EvaluateSelectionStatus()
{
	Nob::Select
	(
		Apply2AllConnectedPipesB
		(
			[&](Pipe const &p) { return p.IsSelected(); }  // if any connected pipe is selected
		)                                                  // knot must also be selected
	);
}

