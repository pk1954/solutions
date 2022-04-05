// InputNeuron.h
//
// NNetModel

#pragma once

#include "MoreTypes.h"
#include "IoNeuron.h"

class Nob;
class BaseKnot;
class DrawContext;
class SignalGenerator;

class InputNeuron : public IoNeuron
{
public:

	explicit InputNeuron(SignalGenerator *, MicroMeterPnt const &);
	explicit InputNeuron(SignalGenerator *, BaseKnot      const &);

	~InputNeuron() final = default;

	void Check() const final;

	void SetSigGen(SignalGenerator * const p) { m_pSigGen = p; }
	SignalGenerator & GetSigGen() { return * m_pSigGen; }

	static bool TypeFits(NobType const type) { return type.IsInputNeuronType(); }

	void Prepare() final;

	void DrawExterior(DrawContext const &, tHighlight const) const final;
	void DrawInterior(DrawContext const &, tHighlight const) const final;

	NobIoMode GetIoMode() const final { return NobIoMode::input; }

	fHertz GetActFrequency() const;

	bool Includes(MicroMeterPnt const &) const final;

	void AppendMenuItems(AddMenuFunc const &) const final;

private:

	SignalGenerator * m_pSigGen { nullptr };

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
