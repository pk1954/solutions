// Shape.h
//
// NNetModel

#pragma once

#include "MoreTypes.h"

using ShapeId = NamedType< unsigned long, struct ShapeIdParam >;

class GraphicsInterface;
class PixelCoordsFp;
class NNetModel;

ShapeId const NO_SHAPE( 0 );

enum class tShapeType
{
	undefined,
	inputNeuron,
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
	return (type == tShapeType::knot) || (type == tShapeType::neuron) || (type == tShapeType::inputNeuron);
}

class Shape
{
public:
	Shape( tShapeType const type )
	  :	m_mVinputBuffer( 0._mV ),
		m_bHighlighted( false ),
		m_identifier( NO_SHAPE ),
		m_type( type )
	{
	}

	virtual void Draw( NNetModel const &, GraphicsInterface &,	PixelCoordsFp const & ) const = 0;
	virtual bool IsPointInShape( NNetModel const &, MicroMeterPoint const & )           const = 0;
	virtual mV   GetNextOutput( )                                                       const = 0;
	virtual void Step( )                                                                      = 0;
	virtual void Prepare( NNetModel & )                                                       = 0;
	virtual void MoveTo( NNetModel &, MicroMeterPoint const & )                               = 0;

	void SetHighlightState( bool const bState )
	{
		m_bHighlighted = bState;
	}

	bool IsHighlighted( ) const
	{
		return m_bHighlighted;
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

protected:
	mV m_mVinputBuffer;

private:
	ShapeId    m_identifier;
	bool       m_bHighlighted;
	tShapeType m_type;
};
