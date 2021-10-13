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

	InputNeuron(MicroMeterPnt const &);
	InputNeuron(BaseKnot      const &);

	virtual ~InputNeuron();

	virtual void Check() const;

	virtual bool operator==(Nob const &) const override;

	static bool const TypeFits(NobType const type) { return type.IsInputNeuronType(); }

	virtual void DrawExterior(DrawContext const &, tHighlight const) const;
	virtual void DrawInterior(DrawContext const &, tHighlight const) const;

	virtual NobIoMode const GetIoMode() const { return NobIoMode::input; }

	virtual void       Prepare();
	virtual bool const CompStep();
	virtual void       DrawNeuronText(DrawContext const &) const;

	fHertz const GetPulseFrequency() const;
	fHertz const SetPulseFrequency(fHertz const);

	virtual bool const Includes(MicroMeterPnt const &) const;

private:

	MicroMeterPnt const getOffset() const;
	MicroMeterPnt const getCenter() const;

	void initialize();

	void drawSocket
	(
		DrawContext  const &, 
		float        const, 
		float        const, 
		D2D1::ColorF const
	) const;

	SignalGenerator m_signalGenerator;
};	
