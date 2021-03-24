// Connector.h
//
// NNetModel

#pragma once

#include "BoolOp.h"
#include "Shape.h"
#include "MoreTypes.h"
#include "ShapeType.h"
#include "UPShapeList.h"
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

	void Push(unique_ptr<ConnectionNeuron> ups) { m_list.push_back( move(ups) ); }
	unique_ptr<ConnectionNeuron> Pop() 
	{ 
		unique_ptr<ConnectionNeuron> upRet { move(m_list.back()) };
		m_list.pop_back();
		return move(upRet);
	}

	size_t const Size()       const { return m_list.size(); }
	bool   const IsEmpty()    const { return m_list.empty(); }
	bool   const IsNotEmpty() const { return ! IsEmpty(); }

	virtual bool operator==(Connector const & rhs) const { return m_list == rhs.m_list;	}

	virtual void       DrawExterior  (DrawContext const &, tHighlightType const) const;
	virtual void       DrawInterior  (DrawContext const &, tHighlightType const) const;
	virtual void       Prepare       ();
	virtual bool const CompStep      ();
	virtual void       Recalc        ();
	virtual void       MoveShape     (MicroMeterPoint const &);
	virtual bool const IsInRect      (MicroMeterRect  const &) const;
	virtual bool const IsPointInShape(MicroMeterPoint const &) const;
	virtual void       Expand        (MicroMeterRect        &) const;

	virtual void Select(tBoolOp const = tBoolOp::opTrue);

	void Apply2All( function<void(unique_ptr<ConnectionNeuron> &)> const & func )
	{
		for (auto & it : m_list) { func( it ); };
	}                        

	void Apply2All( function<void(unique_ptr<ConnectionNeuron> const &)> const & func ) const
	{
		for (auto const & it : m_list) { func( it ); };
	}                        

	friend wostream & operator<< (wostream &, Shape const &);

private:

	vector<unique_ptr<ConnectionNeuron>> m_list {};
};

Connector const * Cast2Connector( Shape const * );
Connector       * Cast2Connector( Shape       * );
