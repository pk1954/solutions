// OutputNeuron.cpp 
//
// NNetModel

#include "stdafx.h"
#include "DrawContext.h"
#include "OutputNeuron.h"

using std::fixed;
using std::wstring;
using std::wostringstream;
using std::fill;
using std::wcout;
using std::endl;

OutputNeuron::OutputNeuron( MicroMeterPoint const & upCenter )
	: IoNeuron( upCenter, NobType::Value::outputNeuron )
{}

OutputNeuron::OutputNeuron( Neuron const & neuron )
	: IoNeuron( neuron.GetPos(), NobType::Value::outputNeuron )
{
	static_cast<Neuron &>(*this) = neuron;      // use all data from Neuron to construct OutputNeuron
	SetType( NobType::Value::outputNeuron );  // fix neuron type
}

void OutputNeuron::Check() const
{
	Neuron::Check();
	assert( ! m_connections.HasOutgoing() );
}

bool OutputNeuron::operator==( Nob const & rhs ) const
{
	return this->Neuron::operator== (static_cast<OutputNeuron const &>(rhs));
}

void OutputNeuron::DrawExterior(DrawContext const & context, tHighlight const type) const
{
	drawPlug( context, 0.8f, 0.8f, GetExteriorColor(type) );
}

void OutputNeuron::DrawInterior(DrawContext const & context, tHighlight const type) const
{
	drawPlug( context, 0.4f, 0.6f, GetInteriorColor(type) );
}

bool const OutputNeuron::Includes(MicroMeterPoint const & point) const
{
	MicroMeterPoint const umCenter { GetPos() + GetScaledDirVector() * 0.5f };
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
	MicroMeterPoint const umDirVector { GetDirVector() };
	MicroMeterPoint const umCenter    { GetPos() - GetScaledDirVector() * 0.1f };
	MicroMeterPoint const umP         { umCenter - umDirVector * (M * 0.5f - 0.2f) };

	context.DrawLine( umCenter + umDirVector * (V + 0.8f), umP, GetExtension() *  M,          colF );
	context.DrawLine( umCenter + umDirVector *  V,         umP, GetExtension() * (M + 1.2f ), colF );
}
