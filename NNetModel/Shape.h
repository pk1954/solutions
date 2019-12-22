// Shape.h
//
// NNetModel

#pragma once

#include "MoreTypes.h"
#include "NNetParameters.h"
#include "tHighlightType.h"
#include "tShapeType.h"

using ShapeId = NamedType< long, struct ShapeIdParam >;

class D2D_driver;
class PixelCoordsFp;
class NNetModel;

ShapeId const NO_SHAPE( -1 );

static bool IsDefined( ShapeId const id ) { return id != NO_SHAPE; }

bool IsPipelineType( tShapeType const );
bool IsNeuronType  ( tShapeType const );
bool IsKnotType    ( tShapeType const );
bool IsBaseKnotType( tShapeType const );

wchar_t const * GetName( tShapeType const );

class Shape
{
public:
	Shape( NNetModel * pModel, tShapeType const type )
	  :	m_pNNetModel( pModel ),
		m_mVinputBuffer( 0._mV ),
		m_identifier( NO_SHAPE ),
		m_type( type )
	{ }

	virtual ~Shape() {}

	static bool TypeFits( tShapeType const type ) {	return true; }  // every shape type is a Shape

	virtual mV   GetNextOutput ( )                                       const = 0;
	virtual void DrawExterior  ( PixelCoordsFp &, tHighlightType const ) const = 0;
	virtual void DrawInterior  ( PixelCoordsFp & )                       const = 0;
	virtual bool IsPointInShape( MicroMeterPoint const & )               const = 0;
	virtual void Prepare       ( )                                             = 0;
	virtual void Step          ( )                                             = 0;

	bool            IsDefined   ( ) const { return ::IsDefined( m_identifier ); }
	wchar_t const * GetName     ( ) const { return ::GetName( m_type ); }
	tShapeType      GetShapeType( ) const { return m_type; }
	ShapeId         GetId       ( ) const { return m_identifier; }

	void SetId( ShapeId const id ) { m_identifier = id;	}

	static tShapeType const GetShapeTypeFromName( wchar_t const * const );
	static void SetGraphics( D2D_driver * const pGraphics ) { m_pGraphics = pGraphics; }

protected:

	NNetModel * m_pNNetModel;
	mV          m_mVinputBuffer;

	static D2D_driver * m_pGraphics;

	COLORREF GetInteriorColor( mV const ) const;
	COLORREF GetInteriorColor( ) const { return GetInteriorColor( m_mVinputBuffer ); }

	float GetFillLevel( mV const ) const;
	float GetFillLevel( ) const { return GetFillLevel( m_mVinputBuffer ); };

	void CheckInputBuffer( ) const;

private:

	ShapeId    m_identifier;
	tShapeType m_type;
};
