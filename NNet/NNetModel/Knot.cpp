// Knot.cpp 
//
// NNetModel

module;

#include <cassert>
#include "Resource.h"

module NNetModel:Knot;

import DrawContext;
import Types;
import :NobType;
import :tHighlight;
import :BaseKnot;
import :PipeList;
import :Pipe;

void Knot::DrawExterior(DrawContext const& context, tHighlight const type) const
{
	FillExternalCircle(context, type);
}

void Knot::DrawInterior(DrawContext const & context, tHighlight const type) const
{
	FillInternalCircle(context, type);
}

void Knot::Check() const
{
	BaseKnot::Check();
	assert(! IsOrphan());
}

void Knot::Emphasize(bool const bOn, bool bDownStream) 
{ 
	Nob::Emphasize(bOn);
	if ((GetNrOfInConns() == 1) && (GetNrOfOutConns() == 1))
	{
		if (bDownStream)
			GetFirstOutgoing().Emphasize(bOn, true);
		else
			GetFirstIncoming().Emphasize(bOn, false);
	}
}
