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
	//if ( idToBeFixed == idLimit )
	//	idToBeFixed = NO_SHAPE;
	//else 
		if ( idToBeFixed > idLimit )
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

static bool IsTerminalType( tShapeType const type )
{
	return  
		(type == tShapeType::inputNeuron) ||
		(type == tShapeType::outputNeuron);
}

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

	static bool TypeFits( tShapeType const type ) {	return true; }  // avery shape type is a Shape

	virtual void DrawExterior  ( PixelCoordsFp  & )        const = 0;
	virtual void DrawInterior  ( PixelCoordsFp  & )        const = 0;
	virtual bool IsPointInShape( MicroMeterPoint const & ) const = 0;
	virtual mV   GetNextOutput ( )                         const = 0;
	virtual void Prepare       ( )                               = 0;
	virtual void Step          ( )                               = 0;
	virtual void MoveTo        ( MicroMeterPoint const & )       = 0;

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

	bool HasAxon( ) const {	return ::HasAxon( GetShapeType( ) ); }

	static tShapeType const GetShapeType( wchar_t const * const );

	static void SetGraphics( GraphicsInterface * const pGraphics )
	{
		m_pGraphics = pGraphics;
	}

protected:

	NNetModel * m_pNNetModel;
	mV          m_mVinputBuffer;

	static GraphicsInterface * m_pGraphics;

	COLORREF GetInteriorColor( mV const ) const;
	COLORREF GetInteriorColor( ) const { return GetInteriorColor( m_mVinputBuffer ); }

	void CheckInputBuffer( ) const;

private:

	ShapeId     m_identifier;
	tShapeType  m_type;
};
