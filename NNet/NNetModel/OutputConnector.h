// OutputConnector.h
//
// NNetModel

#pragma once

#include "NobType.h"
#include "Nob.h"
#include "tHighlightType.h"
#include "IoConnector.h"

using std::unique_ptr;

class IoNeuron;
class DrawContext;

class OutputConnector: public IoConnector
{
public:

	static bool TypeFits(NobType const type) { return type.IsOutputConnectorType(); }

	OutputConnector();
	OutputConnector(vector<IoNeuron *> &);
	OutputConnector(vector<IoNeuron *> &&);

	virtual ~OutputConnector() {}

	NobIoMode GetIoMode() const final { return NobIoMode::output; }

	void DrawExterior(DrawContext const &, tHighlight const) const override;
};
