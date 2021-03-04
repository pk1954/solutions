// OutputNeuron.cpp 
//
// NNetModel

#include "stdafx.h"
#include "DrawContext.h"
#include "OutputNeuron.h"

OutputNeuron::OutputNeuron( MicroMeterPoint const upCenter )
	: Neuron( upCenter, ShapeType::Value::outputNeuron )
{ }

void OutputNeuron::CheckShape( ) const
{
	Neuron::CheckShape();
	assert( ! m_connections.HasOutgoing() );
}

bool OutputNeuron::operator==( Shape const & rhs ) const
{
	return this->Neuron::operator== (static_cast<OutputNeuron const &>(rhs));
}

void OutputNeuron::DrawExterior(DrawContext const & context, tHighlightType const type) const
{
	drawPlug( context, 0.8f, 0.8f, GetExteriorColor(type) );
}

void OutputNeuron::DrawInterior(DrawContext const & context, tHighlightType const type) const
{
	drawPlug( context, 0.4f, 0.6f, GetInteriorColor(type) );
}

void OutputNeuron::drawPlug
( 
	DrawContext  const & context, 
	float        const   M,       // overall width/height                        
	float        const   V,
	D2D1::ColorF const   colF
) const
{
	MicroMeterPoint const umExtVector { DetermineVector(Connections::Type::in) };
	MicroMeterPoint const umCenter    { GetPosition() };

	float           const W           { M + 1.2f };       // width of left/right section                 
	float           const fOffset     { -0.8f };
	MicroMeterPoint const umP1        { umCenter + umExtVector * (V - fOffset) };
	MicroMeterPoint const umP2        { umCenter - umExtVector * (W * 0.5f + fOffset) };

	context.DrawLine( umP1,                      umP2, GetExtension() * M, colF );
	context.DrawLine( umP1 - umExtVector * 0.8f, umP2, GetExtension() * W, colF );
}

