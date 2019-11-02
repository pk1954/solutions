// Shape.h
//
// NNetModel

#pragma once

#include "MoreTypes.h"
#include "NNetParameters.h"

using ShapeId = NamedType< long, struct ShapeIdParam >;

class GraphicsInterface;
class PixelCoordsFp;
class NNetModel;

ShapeId const NO_SHAPE( -1 );

enum class tShapeType
{
	undefined,
	inputNeuron,
	outputNeuron,
	neuron,
	pipeline,
	knot
};

static bool IsDefined( ShapeId const id )
{
	return id != NO_SHAPE;
}

static bool IsBaseKnotType( tShapeType const type )
{
	return 
		(type == tShapeType::knot)        || 
		(type == tShapeType::neuron)      || 
		(type == tShapeType::inputNeuron) ||
		(type == tShapeType::outputNeuron);
}

static bool IsNeuronType( tShapeType const type )
{
	return  
		(type == tShapeType::neuron)      || 
		(type == tShapeType::inputNeuron) ||
		(type == tShapeType::outputNeuron);
}

class Shape
{
public:
	Shape( tShapeType const type )
	  :	m_mVinputBuffer( 0._mV ),
		m_bHighlighted( false ),
		m_bSuperHighlighted( false ),
		m_identifier( NO_SHAPE ),
		m_type( type )
	{
	}

	virtual void DrawExterior  ( )                         const = 0;
	virtual void DrawInterior  ( )                         const = 0;
	virtual bool IsPointInShape( MicroMeterPoint const & ) const = 0;
	virtual void MoveTo        ( MicroMeterPoint const & )       = 0;
	virtual void Prepare       ( )                               = 0;
	virtual void Step( )                                         = 0;
	virtual mV   GetNextOutput( )                          const = 0;

	void SetHighlightState( bool const bState )
	{
		m_bHighlighted = bState;
	}

	void SetSuperHighlightState( bool const bState )
	{
		m_bSuperHighlighted = bState;
	}

	bool IsHighlighted( ) const
	{
		return m_bHighlighted;
	}

	bool IsSuperHighlighted( ) const
	{
		return m_bSuperHighlighted;
	}

	ShapeId GetId( ) const
	{
		return m_identifier;
	}

	void SetId( ShapeId const id )
	{
		m_identifier = id;
	}

	tShapeType GetShapeType( ) const
	{
		return m_type;
	}

	static void SetModel( NNetModel * const pModel	)
	{
		m_pModel = pModel;
	}

	static void SetGraphics( GraphicsInterface * const pGraphics	)
	{
		m_pGraphics = pGraphics;
	}

	static void SetCoordSystem( PixelCoordsFp const * const pCoord )
	{
		m_pCoord = pCoord;
	}

protected:
	mV m_mVinputBuffer;

	static NNetModel           * m_pModel;
	static GraphicsInterface   * m_pGraphics;
	static PixelCoordsFp const * m_pCoord;

	COLORREF GetFrameColor( ) const 
	{ 
		return IsSuperHighlighted( ) 
			   ? EXT_COLOR_SUPER_HIGHLIGHT 
			   : IsHighlighted( ) 
			      ? EXT_COLOR_HIGHLIGHT 
			      : EXT_COLOR_NORMAL;
	};

	COLORREF GetInteriorColor( mV const voltageInput ) const
	{
		mV       const voltage { min( voltageInput, PEAK_VOLTAGE ) };
		int      const colElem { CastToInt( voltage  * 255.0f / PEAK_VOLTAGE) };
		COLORREF const color   { RGB( colElem, 0, 0 ) };
		return color;
	}

	COLORREF GetInteriorColor( ) const
	{
		return GetInteriorColor( m_mVinputBuffer );
	}

private:

	ShapeId    m_identifier;
	bool       m_bHighlighted;
	bool       m_bSuperHighlighted;
	tShapeType m_type;
};
