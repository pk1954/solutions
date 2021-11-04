// InputConnector.h
//
// NNetModel

#pragma once

#include "NobType.h"
#include "Nob.h"
#include "tHighlightType.h"
#include "SignalGenerator.h"
#include "IoConnector.h"

using std::unique_ptr;

class IoNeuron;
class DrawContext;

class InputConnector: public IoConnector
{
public:

	static bool const TypeFits(NobType const type) { return type.IsInputConnectorType(); }

	InputConnector();
	InputConnector(vector<IoNeuron *> &);
	InputConnector(vector<IoNeuron *> &&);

	virtual ~InputConnector() {}

	virtual NobIoMode const GetIoMode() const { return NobIoMode::input; }

	virtual void DrawExterior(DrawContext const &, tHighlight const) const;

	void TriggerStimulus() { m_signalGenerator.TriggerStimulus(); }
	void Tick           () { m_signalGenerator.Tick(); }

	fHertz const GetActFrequency() const { return m_signalGenerator.GetActFrequency(); }

private:

	SignalGenerator m_signalGenerator;
};
