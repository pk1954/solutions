// ShapeType.cpp
//
// NNetModel

#include "stdafx.h"
#include <ostream>
#include <unordered_map>
#include "ShapeType.h"

using std::unordered_map;
using std::wstring;

wstring const ShapeType::GetName( ShapeType::Value const val )
{
	static unordered_map < ShapeType::Value, wstring const > mapShape =
	{
		{ ShapeType::Value::undefined,       L"undefined"       },
		{ ShapeType::Value::connector,       L"connector"       },
		{ ShapeType::Value::closedConnector, L"closedConnector" },
		{ ShapeType::Value::inputNeuron,     L"inputNeuron"     },
		{ ShapeType::Value::outputNeuron,    L"outputNeuron"    },
		{ ShapeType::Value::neuron,          L"neuron"          },
		{ ShapeType::Value::pipe,            L"pipe"            },
		{ ShapeType::Value::knot,            L"knot"            }
	};				  
	return mapShape.at( val );
}

ShapeType::Value const ShapeType::GetTypeFromName( wstring const & name )
{
	static unordered_map < wstring, ShapeType::Value > mapShape =
	{
		{ L"undefined",       ShapeType::Value::undefined       },
		{ L"connector",       ShapeType::Value::connector       },
		{ L"closedConnector", ShapeType::Value::closedConnector },
		{ L"inputNeuron",     ShapeType::Value::inputNeuron     },
		{ L"outputNeuron",    ShapeType::Value::outputNeuron    },
		{ L"neuron",          ShapeType::Value::neuron	        },
	    { L"pipe",            ShapeType::Value::pipe	        },
	    { L"knot",            ShapeType::Value::knot            }
	};				  
	return mapShape.at( name );
}

wostream & operator<< ( wostream & out, ShapeType const & shapeType )
{
	wstring const wstrName { ShapeType::GetName( shapeType.GetValue() ) };
	out << wstrName;
	return out;
}
