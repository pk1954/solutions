// OutputLine.cpp 
//
// NNetModel

module;

#include <cassert>
#include "Resource.h"

module NNetModel:OutputLine;

import Types;
import DrawContext;
import :tHighlight;
import :Synapse;
import :IoLine;
import :PosNob;
import :Knot;
import :Pipe;

OutputLine::OutputLine(MicroMeterPnt const& umPntCenter)
	: IoLine(umPntCenter, NobType::Value::outputLine)
{}

OutputLine::OutputLine(PosNob & posNob)
	: IoLine(posNob.GetPos(), NobType::Value::outputLine)
{
	SetId(posNob.GetId());
	if (posNob.IsSynapse())
		SetPipe(Cast2Synapse(&posNob)->GetAddPipe());
	else if (posNob.IsKnot())
		SetPipe(Cast2Knot(&posNob)->GetIncoming());
}

OutputLine::OutputLine(Pipe& pipe)
	: IoLine(pipe.GetEndPoint(), NobType::Value::outputLine)
{
	SetId(pipe.GetEndNobPtr()->GetId());
	SetPipe(&pipe);
}

void OutputLine::Check() const
{
	PosNob::Check();
}

bool OutputLine::operator==(Nob const & rhs) const
{
	return this->PosNob::operator== (*Cast2OutputLine(&rhs));
}

void OutputLine::Reconnect()
{
	GetPipe()->SetEndPnt(this);
};

void OutputLine::ReplaceIncoming(Pipe* const pDel, Pipe* const pAdd)
{
	if (pDel == GetPipe())
		SetPipe(pAdd);
	else
		assert(false);
}

void OutputLine::SetAllOutgoing(PosNob& src) 
{ 
	assert(false); 
}

void OutputLine::SetAllIncoming(PosNob& src)
{
	SetPipe(Cast2OutputLine(&src)->GetPipe());
}

void OutputLine::DrawExterior(DrawContext const & context, tHighlight const type) const
{
	drawPlug(context, 0.8f, 0.8f, GetExteriorColor(type));
}

void OutputLine::DrawInterior(DrawContext const & context, tHighlight const type) const
{
	drawPlug(context, 0.4f, 0.6f, GetInteriorColor(type));
}

bool OutputLine::Includes(MicroMeterPnt const & point) const
{
	MicroMeterPnt const umCenter { GetPos() + GetScaledDirVector() * 0.5f };
	return Distance(point, umCenter) <= GetExtension();
}

void OutputLine::drawPlug
(
	DrawContext  const & context, 
	float        const   M,       // overall width/height                        
	float        const   V,
	D2D1::ColorF const   colF
) const
{
	MicroMeterPnt const umDirVector { GetDirVector() };
	MicroMeterPnt const umCenter    { GetPos() - GetScaledDirVector() * 0.1f };
	MicroMeterPnt const umP         { umCenter - umDirVector * (M * 0.5f - 0.2f) };

	context.DrawLine(umCenter + umDirVector * (V + 0.8f), umP, GetExtension() *  M,          colF);
	context.DrawLine(umCenter + umDirVector *  V,         umP, GetExtension() * (M + 1.2f), colF);
}

void OutputLine::AppendMenuItems(AddMenuFunc const & add) const
{
	add(IDD_EXTEND_OUTPUTLINE);  // case 11
	IoLine::AppendMenuItems(add);
}

OutputLine const* Cast2OutputLine(Nob const* pNob)
{
	assert(pNob);
	assert(pNob->IsOutputLine());
	return static_cast<OutputLine const*>(pNob);
}

OutputLine* Cast2OutputLine(Nob* pNob)
{
	assert(pNob);
	assert(pNob->IsOutputLine());
	return static_cast<OutputLine*>(pNob);
}
