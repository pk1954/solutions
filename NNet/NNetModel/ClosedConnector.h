// ClosedConnector.h
//
// NNetModel

#pragma once

#include "BoolOp.h"
#include "MoreTypes.h"
#include "MicroMeterPosDir.h"
#include "ShapeType.h"
#include "UPShapeList.h"
#include "ShapePtrList.h"
#include "Connector.h"

class ClosedConnector: public Shape
{
public:

	static bool      const TypeFits(ShapeType const type) { return type.IsClosedConnectorType(); }
	static ShapeType const GetShapeType()                 { return ShapeType::Value::closedConnector; }

	ClosedConnector(Connector &, Connector &);
	virtual ~ClosedConnector() {}

	virtual void CheckShape() const;
	virtual void Dump      () const;

	virtual MicroMeterPoint const GetPos() const;

	virtual void       DrawExterior(DrawContext const &, tHighlight const) const;
	virtual void       DrawInterior(DrawContext const &, tHighlight const) const;
	virtual void       Expand      (MicroMeterRect        &)               const;
	virtual bool const IsIncludedIn(MicroMeterRect  const &)               const;
	virtual bool const Includes    (MicroMeterPoint const &)               const;
	virtual void       RotateShape (MicroMeterPoint const &, Radian const);
	virtual void       MoveShape   (MicroMeterPoint const &);
	virtual void       Select      (bool const, bool const); 
	virtual void       Prepare     ();
	virtual bool const CompStep    ();
	virtual void       Recalc      ();
	virtual void       Clear       ();
	virtual void       Link        (Shape const &, function<Shape * (Shape const *)> const &);

	void SetParentPointers();
	void ClearParentPointers();
	//MicroMeterPosDir const GetPosDir() const;
	//Radian           const GetDir()    const;

	//void SetDir   (Radian           const  );
	//void SetPos   (MicroMeterPoint  const &);
	//void SetPosDir(MicroMeterPosDir const &);

	inline static wchar_t const SEPARATOR     { L':' };
	inline static wchar_t const OPEN_BRACKET  { L'{' };
	inline static wchar_t const CLOSE_BRACKET { L'}' };

private:
	Connector * m_pInputConnector  { nullptr };
	Connector * m_pOutputConnector { nullptr };
};

ClosedConnector const * Cast2ClosedConnector(Shape const *);
ClosedConnector       * Cast2ClosedConnector(Shape       *);
