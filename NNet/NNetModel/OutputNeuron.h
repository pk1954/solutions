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

	~OutputNeuron() final = default;

	void Check() const final;

	bool operator==(Nob const &) const override;

	static bool TypeFits(NobType const type) { return type.IsOutputNeuronType(); }

	void DrawExterior(DrawContext const &, tHighlight const) const override;
	void DrawInterior(DrawContext const &, tHighlight const) const override;

	bool Includes(MicroMeterPnt const &) const final;

	NobIoMode GetIoMode() const final { return NobIoMode::output; }

	void AppendMenuItems(AddMenuFunc const &) const final;

private:

	void drawPlug
	(
		DrawContext  const &, 
		float        const, 
		float        const, 
		D2D1::ColorF const
	) const;
};	
