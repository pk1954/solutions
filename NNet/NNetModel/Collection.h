// Collection.h
//
// NNetModel

#pragma once

#include <vector>
#include "BoolOp.h"
#include "Shape.h"
#include "MoreTypes.h"
#include "ShapeType.h"

using std::vector;
using std::wcout;
using std::endl;

class Collection : public Shape
{
public:

	static bool TypeFits( ShapeType const type ) { return type.IsCollectionType(); }

	Collection() {};
	virtual ~Collection() { }

	virtual void CheckShape() const
	{
		Shape::CheckShape();
		for ( auto & it : m_list )
			it->CheckShape();
	}

	//virtual void Dump() const
	//{
	//	Shape::Dump();
	//	wcout << m_list << endl;
	//}

	size_t const Size()    const { return m_list.size(); }
	bool   const IsEmpty() const { return m_list.empty(); }

	virtual bool operator==(Collection const & rhs) const
	{
		if ( Size() != rhs.Size() )
			return false;
		for ( int i = 0; i < Size(); ++i )
			if ( m_list[i] != rhs.m_list[i] )
				return false;
		return true;
	}

	virtual void IncCounter( ) = 0;
	virtual void DecCounter( ) = 0;

	virtual void DrawExterior  ( DrawContext const &, tHighlightType const ) const = 0;
	virtual void DrawInterior  ( DrawContext const &, tHighlightType const ) const = 0;
	virtual void Prepare       ( )                               = 0;
	virtual bool CompStep      ( )                               = 0;
	virtual void Recalc        ( )                               = 0;
	virtual void MoveShape     ( MicroMeterPoint const & )       = 0;
	virtual bool IsInRect      ( MicroMeterRect  const & ) const = 0;
	virtual bool IsPointInShape( MicroMeterPoint const & ) const = 0;

	virtual void Select(tBoolOp const op = tBoolOp::opTrue) { ApplyOp(m_bSelected, op); }
	virtual void Clear () { m_mVinputBuffer = 0.0_mV; };

	bool IsSelected() const { return m_bSelected; }

	bool            IsDefined   ( ) const { return ::IsDefined( m_identifier ); }
	wchar_t const * GetName     ( ) const { return ShapeType::GetName( m_type.GetValue() ); }
	ShapeType       GetShapeType( ) const { return m_type; }
	ShapeId         GetId       ( ) const { return m_identifier; }

	bool IsPipe        () const { return m_type.IsPipeType        ( ); }
	bool IsKnot        () const { return m_type.IsKnotType        ( ); }
	bool IsNeuron      () const { return m_type.IsNeuronType      ( ); }
	bool IsInputNeuron () const { return m_type.IsInputNeuronType ( ); }
	bool IsAnyNeuron   () const { return m_type.IsAnyNeuronType   ( ); }
	bool IsBaseKnot    () const { return m_type.IsBaseKnotType    ( ); }
	bool IsUndefined   () const { return m_type.IsUndefinedType   ( ); }

	void SetId( ShapeId const id ) { m_identifier = id;	}

	friend wostream & operator<< ( wostream &, Shape const & );

private:

	vector<Shape *> m_list {};
};