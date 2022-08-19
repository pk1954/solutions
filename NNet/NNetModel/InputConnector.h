// InputConnector.h
//
// NNetModel

#pragma once

#include "NobType.h"
#include "IoConnector.h"
#include "NNetParameters.h"
#include "IoLine.h"
#include "InputLine.h"
#include "SignalGenerator.h"

import DrawContext;

using std::unique_ptr;

class InputConnector: public IoConnector
{
public:

	static bool TypeFits(NobType const type) { return type.IsInputConnectorType(); }

	explicit InputConnector(Param &, vector<IoLine *> &&);

	~InputConnector() final = default;

	InputLine & GetElem(size_t const) const;

	void SetSigGen(SignalGenerator * const p)  final;
	SignalGenerator       * GetSigGen ()       final;
	SignalGenerator const * GetSigGenC() const final;

	void Prepare() final { /* */ };

	NobIoMode GetIoMode() const final { return NobIoMode::input; }

	void DrawExterior(DrawContext const &, tHighlight const) const final;
};
