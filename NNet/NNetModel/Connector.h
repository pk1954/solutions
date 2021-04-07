// Connector.h
//
// NNetModel

#pragma once

#include "BoolOp.h"
#include "Compound.h"
#include "MoreTypes.h"
#include "ShapeType.h"
#include "UPShapeList.h"
#include "ShapePtrList.h"
#include "ConnectionNeuron.h"

class Connector : public Compound
{
public:

	static bool      const TypeFits( ShapeType const type ) { return type.IsConnectorType(); }
	static ShapeType const GetShapeType() { return ShapeType::Value::connector; }

	Connector();

	virtual ~Connector() {}

	void AlignDirection();

	virtual MicroMeterPoint const GetPosition () const;
	virtual void                  Prepare     ();
	virtual bool            const CompStep    ();
	virtual void                  Recalc      ();
	virtual bool            const IsIncludedIn(MicroMeterRect  const &) const;
	virtual bool            const Includes    (MicroMeterPoint const &) const;

	void Apply2All(function<void(ConnNeuron const &)> const & func) const;

	inline static wchar_t const SEPARATOR     { L':' };
	inline static wchar_t const OPEN_BRACKET  { L'{' };
	inline static wchar_t const CLOSE_BRACKET { L'}' };

private:
	void apply2All(function<void(ConnNeuron const &)> const &) const;
	void apply2All(function<void(ConnNeuron       &)> const &);
};

Connector const * Cast2Connector( Shape const * );
Connector       * Cast2Connector( Shape       * );
