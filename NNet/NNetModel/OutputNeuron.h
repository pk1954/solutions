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

	explicit OutputNeuron(MicroMeterPnt const &);
	explicit OutputNeuron(BaseKnot      const &);

	virtual ~OutputNeuron() {};

	virtual void Check() const;

	virtual bool operator==(Nob const &) const override;

	static bool TypeFits(NobType const type) { return type.IsOutputNeuronType(); }

	virtual void DrawExterior(DrawContext const &, tHighlight const) const override;
	virtual void DrawInterior(DrawContext const &, tHighlight const) const override;

	virtual bool Includes(MicroMeterPnt const &) const final;

	virtual NobIoMode GetIoMode() const final { return NobIoMode::output; }

private:

	void drawPlug
	(
		DrawContext  const &, 
		float        const, 
		float        const, 
		D2D1::ColorF const
	) const;
};	
