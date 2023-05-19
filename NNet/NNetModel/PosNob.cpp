// PosNob.cpp
//
// NNetModel

module;

#include <memory>
#include <cassert>
#include <iostream>

module NNetModel:PosNob;

import :Knot;
import :InputLine;
import :OutputLine;
import :NNetColors;

using std::wcout;
using std::endl;
using std::unique_ptr;
using std::make_unique;

void PosNob::Dump() const
{
	Nob::Dump();
	//wcout << GetPos() << endl;
}

bool PosNob::operator==(PosNob const& rhs) const
{
//	PosNob const& posNobRhs { static_cast<PosNob const&>(rhs) };
	return (this->Nob::operator==(rhs)) &&
		GetPos().IsCloseTo(rhs.GetPos()) &&
		GetExtension().IsCloseTo(rhs.GetExtension());
}

bool PosNob::Includes(MicroMeterPnt const& point) const
{
	return Distance(point, GetPos()) <= GetExtension();
}

bool PosNob::IsIncludedIn(MicroMeterRect const& umRect) const
{
	return umRect.Includes(GetPos());
}

void PosNob::Expand(MicroMeterRect& umRect) const
{
	umRect.Expand(GetPos());
}

bool PosNob::IsPrecursorOf(Pipe const& pipeSucc) const
{
	bool bRes = Apply2AllOutPipesB([&pipeSucc](Pipe const& pipe) { return &pipe == &pipeSucc; });
	return bRes;
}

bool PosNob::IsSuccessorOf(Pipe const& pipePred) const
{
	bool bRes = Apply2AllInPipesB([&pipePred](Pipe const& pipe) { return &pipe == &pipePred; });
	return bRes;
}

bool PosNob::IsPrecursorOf(PosNob const& b) const
{
	bool bRes = Apply2AllOutPipesB([&b](Pipe const& p) { return p.GetEndNobPtr() == &b; });
	return bRes;
}

bool PosNob::IsSuccessorOf(PosNob const& b) const
{
	bool bRes = Apply2AllInPipesB([&b](Pipe const& p) { return p.GetStartNobPtr() == &b; });
	return bRes;
}

bool PosNob::IsDirectlyConnectedTo(PosNob const& posNob) const
{
	return IsSuccessorOf(posNob) || IsPrecursorOf(posNob);
}

bool PosNob::IsDirectlyConnectedTo(Pipe const& pipe) const
{
	return IsDirectlyConnectedTo(*static_cast<PosNob const*>(pipe.GetStartNobPtr())) ||
           IsDirectlyConnectedTo(*static_cast<PosNob const*>(pipe.GetEndNobPtr()));
}

void PosNob::EvaluateSelectionStatus()
{
	bool bSelected { Apply2AllConnectedPipesB([](Pipe const& p) { return p.IsSelected(); }) };
	Nob::Select(bSelected);      // if any connected pipe is selected
}                                // PosNob must also be selected

bool PosNob::Apply2AllConnectedPipesB(PipeCrit const& c) const
{
	return Apply2AllInPipesB(c) || Apply2AllOutPipesB(c);
}

void PosNob::AttachInputLine(PushFunc const& push, Pipe& pipe)
{
	unique_ptr<InputLine> upInputLine { make_unique<InputLine>(GetPos()) };
	upInputLine->SetPipe(&pipe);
	pipe.SetStartPnt(upInputLine.get());
	push(move(upInputLine));
}

void PosNob::AttachOutputLine(PushFunc const& push, Pipe& pipe)
{
	unique_ptr<OutputLine> upOutputLine { make_unique<OutputLine>(GetPos()) };
	upOutputLine->SetPipe(&pipe);
	pipe.SetEndPnt(upOutputLine.get());
	push(move(upOutputLine));
}

void PosNob::AttachKnot(PushFunc const& push, Pipe& pipeIn, Pipe& pipeOut)
{
	unique_ptr<Knot> upKnot { make_unique<Knot>(GetPos()) };
	upKnot->AddIncoming(&pipeIn);
	upKnot->AddOutgoing(&pipeOut);
	pipeOut.SetStartPnt(upKnot.get());
	pipeIn.SetEndPnt(upKnot.get());
	push(move(upKnot));
}

void PosNob::FillExternalCircle
(
	DrawContext const& context,
	tHighlight  const  type
) const
{
	MicroMeterPnt const umPos { GetPos() };
	MicroMeterCircle    umCircle { umPos, KNOT_WIDTH };
	if (IsEmphasized())
		umCircle *= 2.f;
	switch (type)
	{
	case tHighlight::normal:
		context.FillCircle(umCircle, GetExteriorColor(type));
		break;
	case tHighlight::highlighted:
		context.FillCircle(MicroMeterCircle(umPos, PIPE_WIDTH_HIGH), NNetColors::EXT_HIGHLIGHTED);
		break;
	case tHighlight::targetFit:
		context.FillCircle(MicroMeterCircle(umPos, PIPE_WIDTH_HIGH), NNetColors::EXT_TARGET_FIT);
		break;
	case tHighlight::targetNoFit:
		context.FillCircle(MicroMeterCircle(umPos, PIPE_WIDTH_HIGH), NNetColors::EXT_TARGET_NOFIT);
		break;
	default:
		assert(false);
		break;
	}
}

void PosNob::FillInternalCircle
(
	DrawContext const& context,
	tHighlight  const  type
) const
{
	float  fWidth { PIPE_INTERIOR };
	fPixel fPixMinWidth { 1._fPixel };
	if (IsEmphasized())
	{
		fWidth *= 2.f;
		fPixMinWidth = 3.f;
	}
	MicroMeterCircle const umCircle { GetPos(), KNOT_WIDTH * fWidth };
	switch (type)
	{
	case tHighlight::normal:
		context.FillCircle(umCircle, GetInteriorColor(type, m_mVpotential), fPixMinWidth);
		break;
	case tHighlight::highlighted:
		context.FillCircle(umCircle, GetInteriorColor(type, m_mVpotential), fPixMinWidth);
		break;
	case tHighlight::targetFit:
		context.FillCircle(umCircle, NNetColors::INT_NORMAL, fPixMinWidth);
		break;
	case tHighlight::targetNoFit:
		context.FillCircle(umCircle, NNetColors::INT_NORMAL, fPixMinWidth);
		break;
	case tHighlight::blocked:
		context.FillCircle(umCircle, NNetColors::EXT_NORMAL, fPixMinWidth);
		break;
	default:
		assert(false);
		break;
	}
}

PosNob const* Cast2PosNob(Nob const* pNob)
{
	assert(pNob);
	assert(pNob->IsPosNob());
	return static_cast<PosNob const*>(pNob);
}

PosNob* Cast2PosNob(Nob* pNob)
{
	assert(pNob);
	assert(pNob->IsPosNob());
	return static_cast<PosNob*>(pNob);
}
