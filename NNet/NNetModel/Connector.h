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

	void Push(CNPtr const p) 
	{ 
		m_list.push_back(p); 
	}

	CNPtr const Pop() 
	{ 
		CNPtr pRet { m_list.back() };
		m_list.pop_back();
		return pRet;
	}

	size_t const Size()       const { return m_list.size(); }
	bool   const IsEmpty()    const { return m_list.empty(); }
	bool   const IsNotEmpty() const { return ! IsEmpty(); }

	virtual bool operator==(Connector const & rhs) const { return m_list == rhs.m_list;	}

	virtual MicroMeterPoint const GetPosition   ()                                      const;
	virtual void                  DrawExterior  (DrawContext const &, tHighlight const) const;
	virtual void                  DrawInterior  (DrawContext const &, tHighlight const) const;
	virtual void                  Prepare       ();
	virtual bool            const CompStep      ();
	virtual void                  Recalc        ();
	virtual void                  MoveShape     (MicroMeterPoint const &);
	virtual bool            const IsInRect      (MicroMeterRect  const &) const;
	virtual bool            const IsPointInShape(MicroMeterPoint const &) const;
	virtual void                  Expand        (MicroMeterRect        &) const;

	virtual void Select(bool const, bool const);

	void Apply2All(function<void(CNPtr const &)> const & func) const
	{
		for (CNPtr const & p : m_list) { func(p); };
	}                        

	void Apply2All(function<void(CNPtr &)> const & func)
	{
		for (CNPtr & p : m_list) { func(p); };
	}                        

	inline static wchar_t const SEPARATOR     { L':' };
	inline static wchar_t const OPEN_BRACKET  { L'{' };
	inline static wchar_t const CLOSE_BRACKET { L'}' };

private:

	vector<CNPtr> m_list {};
};

Connector const * Cast2Connector( Shape const * );
Connector       * Cast2Connector( Shape       * );
