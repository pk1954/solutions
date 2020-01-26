// tShapeType.cpp
//
// NNetModel

#include "stdafx.h"
#include <unordered_map>
#include "tShapeType.h"

using std::unordered_map;

void Apply2AllShapeTypes( function<void(tShapeType const &)> const & func )
{
	for ( int i = 0; i <= static_cast<int>(tShapeType::shapeTypeLast); ++i )
		func( static_cast<tShapeType>( i ) );
}

wchar_t const * GetName( tShapeType const type )
{
	static unordered_map < tShapeType, wchar_t const * const > mapShape =
	{
		{ tShapeType::undefined,   L"undefined"   },
		{ tShapeType::inputNeuron, L"inputNeuron" },
		{ tShapeType::neuron,	   L"neuron"      },
		{ tShapeType::pipeline,	   L"pipeline"    },
		{ tShapeType::knot,		   L"knot"        }
	};				  

	return mapShape.at( type );
}

tShapeType const GetShapeTypeFromName( wchar_t const * const name )
{
	static unordered_map < wchar_t const *, tShapeType > mapShape =
	{
		{ L"undefined",   tShapeType::undefined   },
		{ L"inputNeuron", tShapeType::inputNeuron },
		{ L"neuron",      tShapeType::neuron	   },
		{ L"pipeline",    tShapeType::pipeline	   },
		{ L"knot",        tShapeType::knot        }
	};				  

	return mapShape.at( name );
}
