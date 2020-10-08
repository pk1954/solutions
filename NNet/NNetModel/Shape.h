// Shape.h
//
// NNetModel

#pragma once

#include "d2d1helper.h"
#include "BoolOp.h"
#include "MoreTypes.h"
#include "tHighlightType.h"
#include "NNetParameters.h"
#include "ShapeType.h"
#include "ShapeId.h"

class DrawContext;
class NNetModel;

using ShapeCrit = function<bool(Shape const &)>;
                  
static ShapeCrit const ShapeCritAlwaysTrue { [&]( Shape const & s) { return true; } };

class Shape
{
public:
	Shape( ShapeType const type )
		: m_type( type )
	{ }	

	Shape( Shape const & src )   // copy constructor
	  :	m_type      (src.m_type),
		m_identifier(src.m_identifier),
		m_bSelected (src.m_bSelected),
		m_bMarked   (src.m_bMarked)
	{
	}

	virtual void CheckShape() const;

	Shape & operator= ( Shape const & ) = delete;

	virtual bool IsEqual( Shape const & other ) const
	{
		if ( m_type.GetValue() != other.m_type.GetValue() )
			return false;
		if ( m_identifier.GetValue() != other.m_identifier.GetValue() )
			return false;
		if ( m_bSelected != other.m_bSelected )
			return false;
		if ( m_bMarked != other.m_bMarked )
			return false;
		return true;
	}

	virtual ~Shape() { }

	virtual void IncCounter( ) = 0;
	virtual void DecCounter( ) = 0;

	static bool TypeFits( ShapeType const type ) { return true; }  // every shape type is a Shape

	void Mark( tBoolOp const op ) { ApplyOp( m_bMarked, op ); }

	bool IsSelected( ) const { return m_bSelected; }
	bool IsMarked  ( ) const { return m_bMarked; }

	virtual void DrawExterior  ( DrawContext const &, tHighlightType const = tHighlightType::normal ) const = 0;
	virtual void DrawInterior  ( DrawContext const & )     const = 0;
	virtual void Prepare       ( )                               = 0;
	virtual bool CompStep      ( )                               = 0;
	virtual void Recalc        ( )                               = 0;
	virtual void MoveShape     ( MicroMeterPoint const & )       = 0;
	virtual bool IsInRect      ( MicroMeterRect  const & ) const = 0;
	virtual bool IsPointInShape( MicroMeterPoint const & ) const = 0;

	virtual void Select( tBoolOp const op ) { ApplyOp( m_bSelected, op ); }
	virtual void Clear ( )                  { m_mVinputBuffer = 0.0_mV; };

	bool            IsDefined   ( ) const { return ::IsDefined( m_identifier ); }
	wchar_t const * GetName     ( ) const { return ShapeType::GetName( m_type.GetValue() ); }
	ShapeType       GetShapeType( ) const { return m_type; }
	ShapeId         GetId       ( ) const { return m_identifier; }

	bool IsPipe       () const { return m_type.IsPipeType       ( ); }
	bool IsKnot       () const { return m_type.IsKnotType       ( ); }
	bool IsNeuron     () const { return m_type.IsNeuronType     ( ); }
	bool IsInputNeuron() const { return m_type.IsInputNeuronType( ); }
	bool IsAnyNeuron  () const { return m_type.IsAnyNeuronType  ( ); }
	bool IsBaseKnot   () const { return m_type.IsBaseKnotType   ( ); }
	bool IsUndefined  () const { return m_type.IsUndefinedType  ( ); }

	void SetId( ShapeId const id ) { m_identifier = id;	}

	static void SetParam( Param const * const pParam ) { m_pParameters = pParam; }

protected:

	mV m_mVinputBuffer { 0._mV };

	inline static Param const * m_pParameters{ nullptr };

	D2D1::ColorF GetFrameColor( tHighlightType const ) const;
	D2D1::ColorF GetInteriorColor( mV const ) const;
	D2D1::ColorF GetInteriorColor( ) const { return GetInteriorColor( m_mVinputBuffer ); }

	float GetFillLevel( mV const ) const;
	float GetFillLevel( ) const { return GetFillLevel( m_mVinputBuffer ); };

private:

	ShapeType m_type       { ShapeType::Value::undefined };
	ShapeId   m_identifier { NO_SHAPE };
	bool      m_bSelected  { false };
	bool      m_bMarked    { false };
};

template <typename T>
bool IS_EQUAL( Shape const & shapeA, Shape const & shapeB )
{
	return static_cast<T const &>( shapeA ).IsEqual( static_cast<T const &>( shapeB ) );
}

template <typename T> bool HasType( Shape const & shape ) 
{ 
	return remove_pointer<T>::type::TypeFits( shape.GetShapeType() ); 
}
