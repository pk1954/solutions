// Shape.h
//
// NNetModel

#pragma once

#include <type_traits>   
#include "d2d1helper.h"
#include "BoolOp.h"
#include "MoreTypes.h"
#include "tHighlightType.h"
#include "NNetParameters.h"
#include "ShapeType.h"
#include "ShapeId.h"

class Shape;
class DrawContext;

using std::is_base_of;
using std::remove_pointer_t;
using std::remove_pointer;
using std::unique_ptr;
using std::wostream;
using std::wstring;

template <typename T> 
concept Shape_t = is_base_of<Shape, remove_pointer_t<T>>::value;

using UPShape   = unique_ptr<Shape>;
using ShapeCrit = function<bool(Shape const &)>;

static ShapeCrit const ShapeCritAlwaysTrue { [&]( Shape const & s) { return true; } };

class Shape
{
public:
	Shape( ShapeType const );
	virtual ~Shape() { }

	virtual void CheckShape() const;
	virtual void Dump() const;

	virtual bool operator==( Shape const & ) const;
	
	virtual void IncCounter( ) = 0;
	virtual void DecCounter( ) = 0;

	virtual void DrawExterior  ( DrawContext const &, tHighlightType const = tHighlightType::normal ) const = 0;
	virtual void DrawInterior  ( DrawContext const & )     const = 0;
	virtual void Prepare       ( )                               = 0;
	virtual bool CompStep      ( )                               = 0;
	virtual void Recalc        ( )                               = 0;
	virtual void MoveShape     ( MicroMeterPoint const & )       = 0;
	virtual bool IsInRect      ( MicroMeterRect  const & ) const = 0;
	virtual bool IsPointInShape( MicroMeterPoint const & ) const = 0;

	virtual void Select( tBoolOp const op )    { ApplyOp( m_bSelected, op ); }
	virtual void Clear ( )                     { m_mVinputBuffer = 0.0_mV; };
	virtual void AddOffset(long const lOffset) { m_identifier += lOffset; };

	bool IsSelected( ) const { return m_bSelected; }

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

	static bool TypeFits( ShapeType const type ) { return true; }  // every shape type is a Shape

	friend wostream & operator<< ( wostream &, Shape const & );

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
	bool      m_bSelected  { false };
	ShapeId   m_identifier { };
};

template <Shape_t T> bool HasType( Shape const & shape ) 
{ 
	return remove_pointer<T>::type::TypeFits( shape.GetShapeType() ); 
}
