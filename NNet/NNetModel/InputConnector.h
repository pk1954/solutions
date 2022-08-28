// InputConnector.h
//
// NNetModel

#pragma once

#include <vector>
#include "NobType.h"
#include "IoConnector.h"
#include "InputLine.h"
#include "SignalGenerator.h"

import DrawContext;
import NNetParameters;
import IoLine;

using std::unique_ptr;
using std::vector;

class InputConnector: public IoConnector
{
public:

	static bool TypeFits(NobType const type) { return type.IsInputConnectorType(); }

	explicit InputConnector(Param &, vector<IoLine *> &&);

	~InputConnector() final = default;

	InputLine & GetElem(size_t const) const;

	void SetSigGen(SignalGenerator * const);
	SignalGenerator       * GetSigGen ();
	SignalGenerator const * GetSigGenC() const;

	void Prepare() final { /* */ };

	NobIoMode GetIoMode() const final { return NobIoMode::input; }

	void DrawExterior(DrawContext const &, tHighlight const) const final;
};
