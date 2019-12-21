// tShapeType.h
//
// NNetModel

#pragma once

#include <functional>

enum class tShapeType
{
	inputNeuron,
	neuron,
	pipeline,
	knot,
	shapeTypeLast = knot,
	undefined
};

static void Apply2AllShapeTypes( function<void(tShapeType const &)> const & func )
{
	for ( int i = 0; i <= static_cast<int>(tShapeType::shapeTypeLast); ++i )
		func( static_cast<tShapeType>( i ) );
}

class StartKnotType
{
public:
	static bool TypeFits( tShapeType const type ) 
	{
		return 
			(type == tShapeType::knot)        || 
			(type == tShapeType::neuron)      || 
			(type == tShapeType::inputNeuron);
	}
};

class NeuronType
{
public:
	static bool TypeFits( tShapeType const type ) 
	{
		return (type == tShapeType::neuron) || (type == tShapeType::inputNeuron);
	}
};

class EndKnotType
{
public:
	static bool TypeFits( tShapeType const type ) 
	{
		return (type == tShapeType::knot) || (type == tShapeType::neuron);
	}
};

class TerminalType
{
public:
	static bool TypeFits( tShapeType const type ) 
	{
		return  
			(type == tShapeType::inputNeuron);
	}
};

