// Shape.cpp 
//
// NNetModel

#include "stdafx.h"
#include <unordered_map>
#include "NNetModel.h"
#include "shape.h"

using std::unordered_map;

GraphicsInterface * Shape::m_pGraphics { nullptr };

COLORREF Shape::GetInteriorColor
(
	NNetModel const & model,
	mV        const voltageInput 
) const
{
	mV       const voltage { min( voltageInput, mV(model.GetParameterValue( tParameter::peakVoltage )) ) };
	int      const colElem { CastToInt( voltage  * 255.0f / mV(model.GetParameterValue( tParameter::peakVoltage )) ) };
	COLORREF const color   { RGB( colElem, 0, 0 ) };
	return color;
}

void Shape::CheckInputBuffer( NNetModel const & model ) const
{
	assert( m_mVinputBuffer <= mV( model.GetParameterValue( tParameter::peakVoltage ) * 2 ) );
}

wchar_t const * const Shape::GetName( ) const
{
	static unordered_map < tShapeType, wchar_t const * const > mapShape =
	{
		{ tShapeType::undefined,    L"undefined"    },
		{ tShapeType::inputNeuron,	L"inputNeuron"  },
		{ tShapeType::outputNeuron,	L"outputNeuron" },
		{ tShapeType::neuron,		L"neuron"       },
		{ tShapeType::pipeline,		L"pipeline"     },
		{ tShapeType::knot,			L"knot"         }
	};				  

	return mapShape.at( m_type );
}

tShapeType const Shape::GetShapeType( wchar_t const * const name )
{
	static unordered_map < wchar_t const *, tShapeType > mapShape =
	{
		{ L"undefined",    tShapeType::undefined    },
		{ L"inputNeuron",  tShapeType::inputNeuron	},
		{ L"outputNeuron", tShapeType::outputNeuron	},
		{ L"neuron",       tShapeType::neuron		},
		{ L"pipeline",     tShapeType::pipeline		},
		{ L"knot",         tShapeType::knot,		}
	};				  

	return mapShape.at( name );
}
