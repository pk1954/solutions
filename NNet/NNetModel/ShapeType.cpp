// ShapeType.cpp
//
// NNetModel

#include "stdafx.h"
#include <unordered_map>
#include "ShapeType.h"

using std::unordered_map;

wchar_t const * ShapeType::GetName( ShapeType::Value const val )
{
	static unordered_map < ShapeType::Value, wchar_t const * const > mapShape =
	{
		{ ShapeType::Value::undefined,   L"undefined"   },
		{ ShapeType::Value::inputNeuron, L"inputNeuron" },
		{ ShapeType::Value::neuron,      L"neuron"      },
		{ ShapeType::Value::pipe,        L"pipe"    },
		{ ShapeType::Value::knot,        L"knot"        }
	};				  

	return mapShape.at( val );
}

ShapeType::Value const ShapeType::GetTypeFromName( wchar_t const * const name )
{
	static unordered_map < wchar_t const *, ShapeType::Value > mapShape =
	{
		{ L"undefined",   ShapeType::Value::undefined   },
		{ L"inputNeuron", ShapeType::Value::inputNeuron },
		{ L"neuron",      ShapeType::Value::neuron	    },
	    { L"pipe",        ShapeType::Value::pipe	    },
	    { L"knot",        ShapeType::Value::knot        }
	};				  

	return mapShape.at( name );
}
