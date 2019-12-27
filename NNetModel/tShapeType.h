// tShapeType.h
//
// NNetModel

#pragma once

#include <functional>

using std::function;

enum class tShapeType
{
	inputNeuron,
	neuron,
	pipeline,
	knot,
	shapeTypeLast = knot,
	undefined
};

extern void               Apply2AllShapeTypes( function< void( tShapeType const & ) > const & );
extern tShapeType const   GetShapeTypeFromName( wchar_t const * const );
extern wchar_t    const * GetName( tShapeType const );

inline bool IsPipelineType( tShapeType const type )
{
	return type == tShapeType::pipeline;
}

inline bool IsKnotType( tShapeType const type )
{
	return type == tShapeType::knot;
}

inline bool IsNeuronType( tShapeType const type )
{
	return type == tShapeType::neuron;
}

inline bool IsInputNeuronType( tShapeType const type )
{
	return type == tShapeType::inputNeuron;
}

inline bool IsAnyNeuronType( tShapeType const type )
{
	return (type == tShapeType::neuron)      || 
		   (type == tShapeType::inputNeuron);
}

inline bool IsBaseKnotType( tShapeType const type )
{
	return (type == tShapeType::knot)        || 
		   (type == tShapeType::neuron)      || 
		   (type == tShapeType::inputNeuron);
}
