// InputConnector.h
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

class InputConnector: public IoConnector
{
public:

	static bool const TypeFits(NobType const type) { return type.IsInputConnectorType(); }

	InputConnector();
	InputConnector(vector<IoNeuron *> &);
	InputConnector(vector<IoNeuron *> &&);
	InputConnector(InputConnector const &);   // copy constructor

	virtual ~InputConnector() {}

	virtual NobIoMode const GetIoMode() const { return NobIoMode::input; }

	virtual void DrawExterior(DrawContext const &, tHighlight const) const;
};
