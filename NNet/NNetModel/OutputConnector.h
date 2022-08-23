// OutputConnector.h
//
// NNetModel

#pragma once

#include "NobType.h"
#include "Nob.h"
#include "IoConnector.h"
#include "IoLine.h"

import tHighlight;
import DrawContext;

class OutputConnector: public IoConnector
{
public:

	static bool TypeFits(NobType const type) { return type.IsOutputConnectorType(); }

	OutputConnector();
	explicit OutputConnector(vector<IoLine *> &);
	explicit OutputConnector(vector<IoLine *> &&);

	~OutputConnector() final = default;

	void Prepare() override { /* done by output neurons */  };

	NobIoMode GetIoMode() const final { return NobIoMode::output; }

	void DrawExterior(DrawContext const &, tHighlight const) const override;
};
