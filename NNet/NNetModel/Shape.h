// Shape.h
//
// NNetModel

#pragma once

#include <vector>
#include "d2d1helper.h"
#include "MoreTypes.h"
#include "NNetParameters.h"
#include "tHighlightType.h"
#include "tShapeType.h"

class Shape;
class D2D_driver;
class PixelCoordsFp;
class NNetModel;

using ShapeId = NamedType< long, struct ShapeIdParam >;

ShapeId const NO_SHAPE( -1 );

static bool IsDefined( ShapeId const id ) { return id != NO_SHAPE; }

wchar_t const * GetName( tShapeType const );

class Shape
{
public:
	Shape( tShapeType const type )
	  :	m_mVinputBuffer( 0._mV ),
		m_bEmphasized( false ),
		m_identifier( NO_SHAPE ),
		m_type( type )
	{ }

	virtual ~Shape() {}

	static bool TypeFits( tShapeType const type ) {	return true; }  // every shape type is a Shape

	void Emphasize( bool const bState ) { m_bEmphasized = bState; }
	bool IsEmphasized( ) { return m_bEmphasized; }

	virtual bool IsInRect      ( MicroMeterRect const & )                const = 0;
	virtual void DrawExterior  ( PixelCoordsFp &, tHighlightType const ) const = 0;
	virtual void DrawInterior  ( PixelCoordsFp & )                       const = 0;
	virtual bool IsPointInShape( MicroMeterPoint const & )               const = 0;
	virtual void Prepare       ( )                                             = 0;
	virtual void Step          ( )                                             = 0;
	virtual void Recalc        ( )                                             = 0;

	virtual void Clear( ) { m_mVinputBuffer = 0.0_mV; };

	bool            IsDefined   ( ) const { return ::IsDefined( m_identifier ); }
	wchar_t const * GetName     ( ) const { return ::GetName( m_type ); }
	tShapeType      GetShapeType( ) const { return m_type; }
	ShapeId         GetId       ( ) const { return m_identifier; }

	bool IsPipeline   () const { return ::IsPipelineType   ( m_type ); }
	bool IsKnot       () const { return ::IsKnotType       ( m_type ); }
	bool IsNeuron     () const { return ::IsNeuronType     ( m_type ); }
	bool IsInputNeuron() const { return ::IsInputNeuronType( m_type ); }
	bool IsAnyNeuron  () const { return ::IsAnyNeuronType  ( m_type ); }
	bool IsBaseKnot   () const { return ::IsBaseKnotType   ( m_type ); }

	void SetId( ShapeId const id ) { m_identifier = id;	}

	static void SetGraphics( D2D_driver const * const pGraphics ) { m_pGraphics  = pGraphics; }
	static void SetModel   ( NNetModel  const * const pModel    ) { m_pNNetModel = pModel; }

protected:

	mV m_mVinputBuffer;

	static D2D_driver const * m_pGraphics;
	static NNetModel  const * m_pNNetModel;

	D2D1::ColorF GetInteriorColor( mV const ) const;
	D2D1::ColorF GetInteriorColor( ) const { return GetInteriorColor( m_mVinputBuffer ); }

	float GetFillLevel( mV const ) const;
	float GetFillLevel( ) const { return GetFillLevel( m_mVinputBuffer ); };

private:

	bool       m_bEmphasized;
	ShapeId    m_identifier;
	tShapeType m_type;
};
