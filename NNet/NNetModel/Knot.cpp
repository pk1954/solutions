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
import :NNetColors;
import :BaseKnot;
import :PipeList;
import :Pipe;

//Knot::Knot(BaseKnot const & src)
//  : BaseKnot(src)
//{
//	SetType(NobType::Value::knot);
//	SetExtension(KNOT_WIDTH);
//}

void Knot::FillExternalCircle
(
	DrawContext      const & context, 
	MicroMeterCircle const & umCircleParam,
	tHighlight       const   type
) const
{
	MicroMeterCircle    umCircle { umCircleParam };
	MicroMeterPnt const umPos    { umCircle.GetPos() };
	if (IsEmphasized())
		umCircle *= 2.f;
	switch (type)
	{
	case tHighlight::normal:
		context.FillCircle(umCircle, GetExteriorColor(type));
		break;
	case tHighlight::highlighted:
		context.FillCircle(MicroMeterCircle(umPos, 30.0_MicroMeter), NNetColors::EXT_HIGHLIGHTED);
		break;
	case tHighlight::targetFit:
		context.FillCircle(MicroMeterCircle(umPos, 30.0_MicroMeter), NNetColors::EXT_TARGET_FIT);
		break;
	case tHighlight::targetNoFit:
		context.FillCircle(MicroMeterCircle(umPos, 30.0_MicroMeter), NNetColors::EXT_TARGET_NOFIT);
		break;
	default:
		assert(false);
		break;
	}
}

void Knot::DrawExterior(DrawContext const& context, tHighlight const type) const
{
	FillExternalCircle(context, GetCircle(), type);
}

void Knot::FillInternalCircle
(
	DrawContext      const & context,
	MicroMeterCircle const & umCircleParam,
	tHighlight       const   type
) const
{
	float  fWidth       { PIPE_INTERIOR };
	fPixel fPixMinWidth { 1._fPixel };
	if (IsEmphasized())
	{
		fWidth *= 2.f;
		fPixMinWidth = 3.f;
	}
	MicroMeterCircle const umCircle { umCircleParam * fWidth };
	switch (type)
	{
	case tHighlight::normal:
		context.FillCircle(umCircle, GetInteriorColor(type), fPixMinWidth);
		break;
	case tHighlight::highlighted:
		context.FillCircle(umCircle, GetInteriorColor(type), fPixMinWidth);
		break;
	case tHighlight::targetFit:
		context.FillCircle(umCircle, NNetColors::INT_NORMAL, fPixMinWidth);
		break;
	case tHighlight::targetNoFit:
		context.FillCircle(umCircle, NNetColors::INT_NORMAL, fPixMinWidth);
		break;
	default:
		assert(false);
		break;
	}
}

void Knot::DrawInterior(DrawContext const & context, tHighlight const type) const
{
	FillInternalCircle(context, GetCircle(), type);
}

void Knot::Check() const
{
	BaseKnot::Check();
	assert(! IsOrphan());
}

void Knot::EvaluateSelectionStatus()
{
	Nob::Select(AnyConnectedPipesSelected()); // if any connected pipe is selected
}                                             // knot must also be selected

void Knot::AppendMenuItems(AddMenuFunc const & add) const
{
	if ((GetNrOfInConns() == 1) && (GetNrOfOutConns() == 1)) 
	{
	   add(IDD_ADD_INCOMING2BASEKNOT);
	   add(IDD_ADD_OUTGOING2BASEKNOT);
	}

	BaseKnot::AppendMenuItems(add);
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
