// ClosedConnector.h
//
// NNetModel

#pragma once

#include "NobType.h"
#include "Nob.h"
#include "tHighlightType.h"
#include "IoConnector.h"

class ClosedConnector: public IoConnector
{
public:

	static bool const TypeFits(NobType const type) { return type.IsClosedConnectorType(); }

	ClosedConnector() :	IoConnector(NobType::Value::closedConnector) {};
	virtual ~ClosedConnector() {}

	virtual NobIoMode const GetIoMode() const { return NobIoMode::internal; }

	virtual void DrawExterior(DrawContext const &, tHighlight const) const;
};

ClosedConnector & Cast2ClosedConnector(Nob &);
