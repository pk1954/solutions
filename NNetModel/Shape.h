// Shape.h
//
// NNetModel

#pragma once

#include "MoreTypes.h"

using ShapeId = NamedType< unsigned long, struct ShapeIdParam >;

class GraphicsInterface;
class PixelCoordsFp;

ShapeId const NO_SHAPE( 0 );

enum class tShapeType
{
	undefined,
	inputNeuron,
	neuron,
	pipeline,
	knot
};

class Shape
{
public:
	Shape( tShapeType const type )
	  :	m_bHighlighted( false ),
		m_identifier( -1 ),
		m_type( type )
	{
	}

	virtual void Draw( GraphicsInterface &,	PixelCoordsFp const & ) const = 0;
	virtual bool IsPointInShape( MicroMeterPoint const & )          const = 0;
	virtual mV   GetNextOutput( )                                   const = 0;
	virtual void Step( )                                                  = 0;
	virtual void Prepare( )                                               = 0;
	virtual void Move( MicroMeterPoint const & )                          = 0;

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

private:
	ShapeId    m_identifier;
	bool       m_bHighlighted;
	tShapeType m_type;
};
