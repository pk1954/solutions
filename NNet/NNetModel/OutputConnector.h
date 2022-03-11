// OutputConnector.h
//
// NNetModel

#pragma once

#include "NobType.h"
#include "Nob.h"
#include "tHighlightType.h"
#include "IoConnector.h"

class IoNeuron;
class DrawContext;

class OutputConnector: public IoConnector
{
public:

	static bool TypeFits(NobType const type) { return type.IsOutputConnectorType(); }

	OutputConnector();
	explicit OutputConnector(vector<IoNeuron *> &);
	explicit OutputConnector(vector<IoNeuron *> &&);

	~OutputConnector() final = default;

	void Prepare() override { /* done by output neurons */  };

	NobIoMode GetIoMode() const final { return NobIoMode::output; }

	void DrawExterior(DrawContext const &, tHighlight const) const override;
};
