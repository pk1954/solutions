// OutputNeuron.h
//
// NNetModel

#pragma once

#include "MoreTypes.h"
#include "IoNeuron.h"

class Nob;
class Neuron;
class DrawContext;

class OutputNeuron : public IoNeuron
{
public:

	OutputNeuron(MicroMeterPnt const &);
	OutputNeuron(Neuron        const &);

	virtual ~OutputNeuron() {};

	virtual void Check() const;

	virtual bool operator==( Nob const & ) const override;

	static bool    const TypeFits( NobType const type ) { return type.IsOutputNeuronType(); }
//	virtual NobType const GetNobType() { return NobType::Value::outputNeuron; }

	virtual void DrawExterior( DrawContext const &, tHighlight const ) const;
	virtual void DrawInterior( DrawContext const &, tHighlight const ) const;

	virtual bool const Includes( MicroMeterPnt const & ) const;

	virtual NobIoMode const GetIoMode() const { return NobIoMode::output; }

private:

	void drawPlug
	( 
		DrawContext  const &, 
		float        const, 
		float        const, 
		D2D1::ColorF const
	) const;
};	
