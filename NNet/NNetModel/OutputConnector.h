// OutputConnector.h
//
// NNetModel

#pragma once

#include "MoreTypes.h"
#include "NobType.h"
#include "Nob.h"
#include "IoConnector.h"

using std::unique_ptr;

class OutputConnector: public IoConnector
{
public:

	static bool const TypeFits(NobType const type) { return type.IsOutputConnectorType(); }

	OutputConnector();
	OutputConnector(unique_ptr<IoNeuronList>);
	OutputConnector(OutputConnector const &);   // copy constructor

	virtual ~OutputConnector() {}

	virtual NobIoMode const GetIoMode() const { return NobIoMode::output; }

	virtual void DrawExterior(DrawContext const &, tHighlight const) const;
};
