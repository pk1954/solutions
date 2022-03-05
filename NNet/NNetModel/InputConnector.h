// InputConnector.h
//
// NNetModel

#pragma once

#include "NobType.h"
#include "Nob.h"
#include "tHighlightType.h"
#include "SignalGenerator.h"
#include "IoConnector.h"

class IoNeuron;
class DrawContext;

class InputConnector: public IoConnector
{
public:

	static bool TypeFits(NobType const type) { return type.IsInputConnectorType(); }

	explicit InputConnector(vector<IoNeuron *> &&);

	~InputConnector() final = default;

	void AppendMenuItems(AddMenuFunc const &) const final;

	NobIoMode GetIoMode() const final { return NobIoMode::input; }

	void DrawExterior(DrawContext const &, tHighlight const) const final;

	void TriggerStimulus() { m_signalGenerator.TriggerStimulus(); }
	void Tick           () { m_signalGenerator.Tick(); }

	SignalGenerator       & GetSignalGenerator ()       { return m_signalGenerator; }
	SignalGenerator const & GetSignalGeneratorC() const { return m_signalGenerator; }

	fHertz GetActFrequency() const { return m_signalGenerator.GetActFrequency(); }

	mV WaveFunction(fMicroSecs const) const;

private:

	SignalGenerator m_signalGenerator;
};
