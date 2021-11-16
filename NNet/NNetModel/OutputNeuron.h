// OutputNeuron.h
//
// NNetModel

#pragma once

#include "MoreTypes.h"
#include "IoNeuron.h"

class Nob;
class BaseKnot;
class DrawContext;

class OutputNeuron : public IoNeuron
{
public:

	OutputNeuron(MicroMeterPnt const &);
	OutputNeuron(BaseKnot      const &);

	virtual ~OutputNeuron() {};

	virtual void Check() const;

	virtual bool operator==(Nob const &) const override;

	static bool const TypeFits(NobType const type) { return type.IsOutputNeuronType(); }

	virtual void DrawExterior(DrawContext const &, tHighlight const) const;
	virtual void DrawInterior(DrawContext const &, tHighlight const) const;

	virtual bool Includes(MicroMeterPnt const &) const;

	virtual NobIoMode GetIoMode() const { return NobIoMode::output; }

private:

	void drawPlug
	(
		DrawContext  const &, 
		float        const, 
		float        const, 
		D2D1::ColorF const
	) const;
};	
