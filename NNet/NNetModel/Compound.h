// Compound.h
//
// NNetModel

#pragma once

#include "Shape.h"
#include "ShapePtrList.h"

class DrawContext;

class Compound : public Shape
{
public:
    Compound(ShapeType const type)
      :	Shape(type)
    {};

    virtual ~Compound() {}

	virtual void CheckShape() const;
	virtual void Dump      () const;

	void SetParentPointers();
	void ClearParentPointers();
	void Clear() { m_list.Clear(); }

	void Push(Shape * const p) 
	{ 
		m_list.Add(p); 
	}

	Shape * const Pop() 
	{ 
		Shape * pRet { & m_list.GetLast() };
		m_list.RemoveLast();
		return pRet;
	}

	Shape const & GetFirst() const { return m_list.GetFirst(); }
	Shape const & GetLast () const { return m_list.GetLast (); }

	void Apply2All(function<void(Shape const &)> const &) const;
	void Apply2All(function<void(Shape       &)> const &);

	size_t const Size()       const { return m_list.Size(); }
    bool   const IsEmpty()    const { return m_list.IsEmpty(); }
    bool   const IsNotEmpty() const { return ! IsEmpty(); }

    virtual bool operator==(Compound const & rhs) const { return m_list == rhs.m_list; }

	virtual void DrawExterior(DrawContext const &, tHighlight const) const;
	virtual void DrawInterior(DrawContext const &, tHighlight const) const;
	virtual void Expand      (MicroMeterRect        &)               const;
	virtual void MoveShape   (MicroMeterPoint const &);
	virtual void Select      (bool const, bool const);

	ShapePtrList<Shape> const & GetList() { return m_list; }

	void Rotate(MicroMeterPoint const &, Radian const);

private:

    ShapePtrList<Shape> m_list {};
};
