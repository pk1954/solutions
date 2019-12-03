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

static void FixShapeId( ShapeId & idToBeFixed, ShapeId const idLimit )
{
	if ( idToBeFixed == idLimit )
		idToBeFixed = NO_SHAPE;
	else if ( idToBeFixed > idLimit )
		--idToBeFixed;
}

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

static bool IsPipelineType( tShapeType const type )
{
	return type == tShapeType::pipeline;
}

static bool IsKnotType( tShapeType const type )
{
	return type == tShapeType::knot;
}

static bool IsBaseKnotType( tShapeType const type )
{
	return 
		(type == tShapeType::knot)        || 
		(type == tShapeType::neuron)      || 
		(type == tShapeType::inputNeuron) ||
		(type == tShapeType::outputNeuron);
}

static bool HasAxon( tShapeType const type )
{
	return 
		(type == tShapeType::neuron)      || 
		(type == tShapeType::inputNeuron);
}

static bool IsStartKnotType( tShapeType const type )
{
	return 
		(type == tShapeType::knot)        || 
		(type == tShapeType::neuron)      || 
		(type == tShapeType::inputNeuron);
}

static bool IsEndKnotType( tShapeType const type )
{
	return 
		(type == tShapeType::knot)        || 
		(type == tShapeType::neuron)      || 
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

	virtual void DrawExterior  ( NNetModel const &, PixelCoordsFp  & )        const = 0;
	virtual void DrawInterior  ( NNetModel const &, PixelCoordsFp  & )        const = 0;
	virtual bool IsPointInShape( NNetModel const &, MicroMeterPoint const & ) const = 0;
	virtual mV   GetNextOutput ( NNetModel const & )                          const = 0;
	virtual void Prepare       ( NNetModel const & )                                = 0;
	virtual void Step          ( NNetModel const & )                                = 0;
	virtual void MoveTo        ( NNetModel       &, MicroMeterPoint const & )       = 0;

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

	bool IsDefined( ) const
	{
		return ::IsDefined( m_identifier );
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

	virtual void FixShapeIds( ShapeId const idLimit )
	{
		::FixShapeId( m_identifier, idLimit );
	}
		
	wchar_t const * const GetName( ) const;

	static tShapeType const GetShapeType( wchar_t const * const );

	static void SetGraphics( GraphicsInterface * const pGraphics )
	{
		m_pGraphics = pGraphics;
	}

protected:

	mV m_mVinputBuffer;

	static GraphicsInterface * m_pGraphics;

	COLORREF GetFrameColor( ) const;
	COLORREF GetInteriorColor( NNetModel const &, mV const ) const;

	COLORREF GetInteriorColor( NNetModel const & model ) const
	{
		return GetInteriorColor( model, m_mVinputBuffer );
	}

private:

	ShapeId    m_identifier;
	bool       m_bHighlighted;
	bool       m_bSuperHighlighted;
	tShapeType m_type;
};
