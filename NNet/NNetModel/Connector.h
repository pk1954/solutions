// Connector.h
//
// NNetModel

#pragma once

#include "BoolOp.h"
#include "Shape.h"
#include "MoreTypes.h"
#include "ShapeType.h"
#include "UPShapeList.h"
#include "ShapePtrList.h"
#include "ConnectionNeuron.h"

using std::vector;

class Connector : public Shape
{
public:

	static bool      const TypeFits( ShapeType const type ) { return type.IsConnectorType(); }
	static ShapeType const GetShapeType() { return ShapeType::Value::connector; }

	Connector();

	virtual ~Connector() {}

	virtual void CheckShape() const;
	virtual void Dump      () const;

	void Clear() { m_list.Clear(); }

	void Push(ConnNeuron * const p) 
	{ 
		m_list.Add(p); 
	}

	ConnNeuron * const Pop() 
	{ 
		ConnNeuron * pRet { & m_list.GetLast() };
		m_list.RemoveLast();
		return pRet;
	}

	void SetParentPointers();
	void ClearParentPointers();
	void AlignDirection();

	size_t const Size()       const { return m_list.Size(); }
	bool   const IsEmpty()    const { return m_list.IsEmpty(); }
	bool   const IsNotEmpty() const { return ! IsEmpty(); }

	virtual bool operator==(Connector const & rhs) const { return m_list == rhs.m_list;	}

	virtual MicroMeterPoint const GetPosition ()                                      const;
	virtual void                  DrawExterior(DrawContext const &, tHighlight const) const;
	virtual void                  DrawInterior(DrawContext const &, tHighlight const) const;
	virtual void                  Prepare     ();
	virtual bool            const CompStep    ();
	virtual void                  Recalc      ();
	virtual void                  MoveShape   (MicroMeterPoint const &);
	virtual bool            const Includes    (MicroMeterRect  const &) const;
	virtual bool            const Includes    (MicroMeterPoint const &) const;
	virtual void                  Expand      (MicroMeterRect        &) const;

	virtual void Select(bool const, bool const);

	void Apply2All(function<void(ConnNeuron const *)> const &) const;

	inline static wchar_t const SEPARATOR     { L':' };
	inline static wchar_t const OPEN_BRACKET  { L'{' };
	inline static wchar_t const CLOSE_BRACKET { L'}' };

private:

	ShapePtrList<ConnNeuron> m_list {};
};

Connector const * Cast2Connector( Shape const * );
Connector       * Cast2Connector( Shape       * );
