// InputConnector.h
//
// NNetModel

#pragma once

#include "MoreTypes.h"
#include "NobType.h"
#include "Nob.h"
#include "IoConnector.h"

using std::unique_ptr;

class InputConnector: public IoConnector
{
public:

	static bool    const TypeFits(NobType const type) { return type.IsInputConnectorType(); }
//	virtual NobType const GetNobType()                 { return NobType::Value::inputConnector; }

	InputConnector();
	InputConnector(unique_ptr<IoNeuronList>);
	InputConnector(InputConnector const &);   // copy constructor

	virtual ~InputConnector() {}

	virtual NobIoMode const GetIoMode() const { return NobIoMode::input; }

	virtual void DrawExterior(DrawContext const &, tHighlight const) const;
};
