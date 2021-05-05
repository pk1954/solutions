// Connector.h
//
// NNetModel

#pragma once

#include "BoolOp.h"
#include "MoreTypes.h"
#include "MicroMeterPosDir.h"
#include "ShapeType.h"
#include "UPShapeList.h"
#include "ShapePtrList.h"
#include "ConnectionNeuron.h"

class Connector: public Shape
{
public:

	static bool      const TypeFits( ShapeType const type ) { return type.IsConnectorType(); }
	static ShapeType const GetShapeType()                   { return ShapeType::Value::connector; }

	Connector();
	Connector(ShapePtrList<ConnNeuron> const &);

	virtual ~Connector() {}

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

	void Push(ConnNeuron * const p) { m_list.Add(p); }

	ConnNeuron * const Pop() 
	{ 
		ConnNeuron * pRet { & m_list.GetLast() };
		m_list.RemoveLast();
		return pRet;
	}

	size_t const Size() const { return m_list.Size(); }

	void SetParentPointers();
	void ClearParentPointers();
	void AlignDirection();

//	void Rotate(MicroMeterPoint const &, Radian const);
	void Rotate(MicroMeterPoint const &, MicroMeterPoint const &);

	MicroMeterPosDir const GetPosDir() const;
	Radian           const GetDir()    const;

	void SetDir   (Radian           const  );
	void SetPos   (MicroMeterPoint  const &);
	void SetPosDir(MicroMeterPosDir const &);

	void Apply2All(function<void(ConnNeuron const &)> const & func) const;

	inline static wchar_t const SEPARATOR     { L':' };
	inline static wchar_t const OPEN_BRACKET  { L'{' };
	inline static wchar_t const CLOSE_BRACKET { L'}' };

private:
	ShapePtrList<ConnNeuron> m_list {};
};

Connector const * Cast2Connector( Shape const * );
Connector       * Cast2Connector( Shape       * );
