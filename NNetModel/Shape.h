// Shape.h
//
// NNetModel

#pragma once

#include "MoreTypes.h"

using ShapeId = NamedType< unsigned long, struct ShapeIdParam >;

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

	virtual bool IsPointInShape( MicroMeterPoint const & point ) const = 0;

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
