// Connector.h
//
// NNetModel

#pragma once

#include "BoolOp.h"
#include "MoreTypes.h"
#include "tHighlightType.h"
#include "NNetParameters.h"

class Connector
{
public:

	static void Initialize( Param const & param ) { m_pParameters = & param; }
	static bool TypeFits( ShapeType const type ) { return true; }  // every shape type is a Shape

	Connector( ShapeType const );
	virtual ~Connector() { }

	virtual void CheckShape() const;
	virtual void Dump() const;

	virtual bool operator==( Shape const & ) const;

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

	inline static Param const * m_pParameters{ nullptr };

};