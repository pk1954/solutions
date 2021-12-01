// InputNeuron.h
//
// NNetModel

#pragma once

#include "MoreTypes.h"
#include "IoNeuron.h"

class Nob;
class BaseKnot;
class DrawContext;

class InputNeuron : public IoNeuron
{
public:

	explicit InputNeuron(MicroMeterPnt const &);
	explicit InputNeuron(BaseKnot      const &);

	~InputNeuron() final = default;

	void Check() const final;

	static bool TypeFits(NobType const type) { return type.IsInputNeuronType(); }

	fHertz GetActFrequency() const;

	void DrawExterior(DrawContext const &, tHighlight const) const final;
	void DrawInterior(DrawContext const &, tHighlight const) const final;

	NobIoMode GetIoMode() const final { return NobIoMode::input; }

	void Prepare() final;
	bool Includes(MicroMeterPnt const &) const final;

	void AppendMenuItems(AddMenuFunc const &) const final;

private:

	MicroMeterPnt getOffset() const;
	MicroMeterPnt getCenter() const;

	void drawSocket
	(
		DrawContext  const &, 
		float        const, 
		float        const, 
		D2D1::ColorF const
	) const;
};	
