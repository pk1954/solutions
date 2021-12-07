// Knot.cpp 
//
// NNetModel

#include "stdafx.h"
#include <assert.h>
#include "Resource.h"
#include "tHighlightType.h"
#include "DrawContext.h"
#include "Knot.h"

Knot::Knot(BaseKnot const & src)
	: BaseKnot(src)
{
	SetType(NobType::Value::knot);
}

void Knot::DrawExterior(DrawContext const & context, tHighlight const type) const
{
	MicroMeter umWidth { GetExtension() };
	if (IsEmphasized())
		umWidth *= 2.f;
	switch (type)
	{
	case tHighlight::normal:
		context.FillCircle(MicroMeterCircle(GetPos(), umWidth), GetExteriorColor(type));
		break;
	case tHighlight::highlighted:
		context.FillCircle(MicroMeterCircle(GetPos(), 30.0_MicroMeter), NNetColors::EXT_HIGHLIGHTED);
		break;
	case tHighlight::targetFit:
		context.FillCircle(MicroMeterCircle(GetPos(), 30.0_MicroMeter), NNetColors::EXT_TARGET_FIT);
		break;
	case tHighlight::targetNoFit:
		context.FillCircle(MicroMeterCircle(GetPos(), 30.0_MicroMeter), NNetColors::EXT_TARGET_NOFIT);
		break;
	default:
		assert(false);
		break;
	}
}

void Knot::DrawInterior(DrawContext const & context, tHighlight const type) const
{
	float  fWidth       { PIPE_INTERIOR };
	fPixel fPixMinWidth { 1._fPixel };
	if (IsEmphasized())
	{
		fWidth      *= 2.f;
		fPixMinWidth = 3.f;
	}
	switch (type)
	{
	case tHighlight::normal:
		context.FillCircle(GetCircle() * fWidth, GetInteriorColor(type), fPixMinWidth);
		break;
	case tHighlight::highlighted:
		context.FillCircle(GetCircle() * fWidth, GetInteriorColor(type), fPixMinWidth);
		break;
	case tHighlight::targetFit:
		context.FillCircle(GetCircle() * fWidth, NNetColors::INT_NORMAL, fPixMinWidth);
		break;
	case tHighlight::targetNoFit:
		context.FillCircle(GetCircle() * fWidth, NNetColors::INT_NORMAL, fPixMinWidth);
		break;
	default:
		assert(false);
		break;
	}
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

void Knot::AppendMenuItems(AddMenuFunc const & add) const
{
	if (
		((GetNrOfIncomingConnections() <= 1) && (GetNrOfOutgoingConnections() <= 1)) ||
		((GetNrOfIncomingConnections() <= 2) && (GetNrOfOutgoingConnections() == 0)) 
	   ) add(IDD_ADD_OUTGOING2KNOT);

	if (
		((GetNrOfIncomingConnections() <= 1) && (GetNrOfOutgoingConnections() <= 1)) ||
		((GetNrOfIncomingConnections() == 0) &&	(GetNrOfOutgoingConnections() <= 2)) 
	   ) add(IDD_ADD_INCOMING2KNOT);

	if ((GetNrOfIncomingConnections() == 0) && (GetNrOfOutgoingConnections() <= 1))
		add(IDD_APPEND_INPUT_NEURON);

	if (! HasOutgoing()) 
		add(IDD_APPEND_OUTPUT_NEURON);

	BaseKnot::AppendMenuItems(add);
}

void Knot::Emphasize(bool const bOn, bool bDownStream) 
{ 
	Nob::Emphasize(bOn);
	if ((GetNrOfIncomingConnections() == 1) && (GetNrOfOutgoingConnections() == 1))
	{
		if (bDownStream)
			GetFirstOutgoing().Emphasize(bOn, true);
		else
			GetFirstIncoming().Emphasize(bOn, false);
	}
}
