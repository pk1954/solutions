// OutputNeuron.cpp 
//
// NNetModel

#include "stdafx.h"
#include "Resource.h"
#include "DrawContext.h"
#include "OutputNeuron.h"

OutputNeuron::OutputNeuron(MicroMeterPnt const & upCenter)
	: IoNeuron(upCenter, NobType::Value::outputNeuron)
{}

OutputNeuron::OutputNeuron(BaseKnot const & baseKnot)
	: IoNeuron(baseKnot, NobType::Value::outputNeuron)
{
	SetIncoming(baseKnot);
}

void OutputNeuron::Check() const
{
	Neuron::Check();
	assert(!HasOutgoing());
}

bool OutputNeuron::operator==(Nob const & rhs) const
{
	return this->Neuron::operator== (static_cast<OutputNeuron const &>(rhs));
}

void OutputNeuron::DrawExterior(DrawContext const & context, tHighlight const type) const
{
	drawPlug(context, 0.8f, 0.8f, GetExteriorColor(type));
}

void OutputNeuron::DrawInterior(DrawContext const & context, tHighlight const type) const
{
	drawPlug(context, 0.4f, 0.6f, GetInteriorColor(type));
}

bool OutputNeuron::Includes(MicroMeterPnt const & point) const
{
	MicroMeterPnt const umCenter { GetPos() + GetScaledDirVector() * 0.5f };
	return Distance(point, umCenter) <= GetExtension();
}

void OutputNeuron::drawPlug
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

void OutputNeuron::AppendMenuItems(AddMenuFunc const & add) const
{
	add(IDD_ADD_OUTGOING2BASEKNOT);
	IoNeuron::AppendMenuItems(add);
}
