// InputLine.cpp 
//
// NNetModel

module;

#include <cassert>

module NNetModel:InputLine;

import Direct2D;
import DrawContext;
import :tHighlight;
import :NobType;
import :Neuron;
import :IoLine;
import :PosNob;
import :Knot;
import Resource;

InputLine::InputLine(MicroMeterPnt const& upCenter)
  : IoLine(upCenter)
{ }

InputLine::InputLine(PosNob & posNob)
  : IoLine(posNob.GetPos())
{
	SetId(posNob.GetId());
	if (posNob.IsKnot())
		SetPipe(Cast2Knot(&posNob)->GetOutgoing());
	else if (posNob.IsNeuron())
		SetPipe(Cast2Neuron(&posNob)->GetAxon());
}

InputLine::InputLine(Pipe& pipe)
	: IoLine(pipe.GetStartPoint())
{
	SetId(pipe.GetStartNobPtr()->GetId());
	SetPipe(&pipe);
}

void InputLine::Check() const
{
	IoLine::Check();
}

void InputLine::Reconnect()
{
	GetPipe()->SetStartPnt(this);
	GetPipe()->PosChanged();
};

void InputLine::ReplaceOutgoing(Pipe* const pDel, Pipe* const pAdd)
{
	if (pDel == GetPipe())
		SetPipe(pAdd);
	else
		assert(false);
}

void InputLine::SetAllIncoming(PosNob& src)
{
	assert(false);
}

void InputLine::SetAllOutgoing(PosNob& src)
{
	SetPipe(Cast2InputLine(&src)->GetPipe());
}

void InputLine::SetAllOutgoing(Neuron& src)
{
	SetPipe(src.GetAxon());
}

void InputLine::DrawExterior(DrawContext const & context, tHighlight const type) const
{
	drawSocket(context, 2.0f, 0.1f, GetExteriorColor(type));
}

void InputLine::DrawInterior(DrawContext const & context, tHighlight const type) const
{
	drawSocket(context, 1.6f, 0.0f, GetInteriorColor(type, m_mVpotential));
}

MicroMeterPnt InputLine::getOffset() const
{
	return GetDirVector() * 0.7f;
}

MicroMeterPnt InputLine::getCenter() const
{
	return GetPos() - getOffset();
}

void InputLine::drawSocket
(
	DrawContext const & context, 
	float       const   M,       // overall width/height                        
	float       const   VEM,     // vertical offset of end point middle section 
	Color       const   colF
) const
{
	MicroMeterPnt  const umDirVector   { GetDirVector() * M };
	MicroMeter     const umSize        { GetExtension() * M };
	MicroMeterPnt  const umOrthoVector { getOffset().OrthoVector() };
	MicroMeterPnt  const umDirVectorVS { umDirVector * 0.5f };
	MicroMeter     const umWidthLR     { umSize - GetExtension() * 1.4f };  // width of left/right section                 
	MicroMeterPnt  const umCenter      { getCenter() };
	MicroMeterPnt  const umStart       { umCenter + umDirVectorVS };
	MicroMeterPnt  const umEndLR       { umCenter - umDirVectorVS };
	MicroMeterPnt  const umEndCenter   { umCenter - umDirVector * VEM };
	MicroMeterLine const umLine        { umStart, umEndLR };

	context.DrawLine(umStart, umEndCenter,   umSize,    colF);
	context.DrawLine(umLine + umOrthoVector, umWidthLR, colF);
	context.DrawLine(umLine - umOrthoVector, umWidthLR, colF);
}

void InputLine::CollectInput()
{
	m_mVpotential = m_pSigGen->GetSignalValue();
}

void InputLine::AppendMenuItems(AddMenuFunc const & add) const
{
	add(IDD_EXTEND_INPUTLINE);   // case 10
	IoLine::AppendMenuItems(add);
}

InputLine const* Cast2InputLine(Nob const* pNob)
{
	assert(pNob);
	assert(pNob->IsInputLine());
	return static_cast<InputLine const*>(pNob);
}

InputLine* Cast2InputLine(Nob* pNob)
{
	assert(pNob);
	assert(pNob->IsInputLine());
	return static_cast<InputLine*>(pNob);
}
