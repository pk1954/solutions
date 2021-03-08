// Connector.h
//
// NNetModel

#pragma once

#include "BoolOp.h"
#include "Shape.h"
#include "MoreTypes.h"
#include "ShapeType.h"
#include "ShapePtrList.h"

using std::vector;
using std::wcout;
using std::endl;

class Connector : public Shape
{
public:

	static bool TypeFits( ShapeType const type ) { return type.IsConnectorType(); }

	Connector() {};
	virtual ~Connector() { }

	virtual void CheckShape() const
	{
		Shape::CheckShape();
		m_list.Check();
	}

	virtual void Dump() const
	{
		Shape::Dump();
		wcout << m_list << endl;
	}

	size_t const Size()    const { return m_list.Size(); }
	bool   const IsEmpty() const { return m_list.IsEmpty(); }

	virtual bool operator==(Connector const & rhs) const
	{
		return m_list == rhs.m_list;
	}

	virtual void IncCounter( ) = 0;
	virtual void DecCounter( ) = 0;

	virtual void       DrawExterior  ( DrawContext const &, tHighlightType const ) const = 0;
	virtual void       DrawInterior  ( DrawContext const &, tHighlightType const ) const = 0;
	virtual void       Prepare       ( )                               = 0;
	virtual bool const CompStep      ( )                               = 0;
	virtual void       Recalc        ( )                               = 0;
	virtual void       MoveShape     ( MicroMeterPoint const & )       = 0;
	virtual bool const IsInRect      ( MicroMeterRect  const & ) const = 0;
	virtual bool       IsPointInShape( MicroMeterPoint const & ) const = 0;

	friend wostream & operator<< ( wostream &, Shape const & );

private:

	ShapePtrList<Shape> m_list {};
};