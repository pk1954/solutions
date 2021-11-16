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

	static bool const TypeFits(NobType const type) { return type.IsOutputConnectorType(); }

	OutputConnector();
	OutputConnector(vector<IoNeuron *> &);
	OutputConnector(vector<IoNeuron *> &&);

	virtual ~OutputConnector() {}

	virtual NobIoMode GetIoMode() const { return NobIoMode::output; }

	virtual void DrawExterior(DrawContext const &, tHighlight const) const;
};
