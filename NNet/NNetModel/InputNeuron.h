// InputNeuron.h
//
// NNetModel

#pragma once

#include "MoreTypes.h"
#include "SignalGenerator.h"
#include "IoNeuron.h"

class Nob;
class BaseKnot;
class DrawContext;

using std::shared_ptr;

class InputNeuron : public IoNeuron
{
public:

	explicit InputNeuron(MicroMeterPnt const &);
	explicit InputNeuron(BaseKnot      const &);

	virtual ~InputNeuron();

	virtual void Check() const;

	static bool TypeFits(NobType const type) { return type.IsInputNeuronType(); }

	fHertz const GetActFrequency() const;

	virtual void DrawExterior(DrawContext const &, tHighlight const) const;
	virtual void DrawInterior(DrawContext const &, tHighlight const) const;

	virtual NobIoMode GetIoMode() const { return NobIoMode::input; }

	virtual void Prepare();
	virtual bool Includes(MicroMeterPnt const &) const;

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
