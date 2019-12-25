// Shape.cpp 
//
// NNetModel

#include "stdafx.h"
#include <unordered_map>
#include "NNetModel.h"
#include "shape.h"

using std::unordered_map;

D2D_driver * Shape::m_pGraphics { nullptr };

COLORREF Shape::GetInteriorColor( mV const voltageInput ) const
{
	mV       const voltage { min( voltageInput, mV(m_pNNetModel->GetParameterValue( tParameter::peakVoltage )) ) };
	int      const colElem { CastToInt( voltage  * 255.0f / mV(m_pNNetModel->GetParameterValue( tParameter::peakVoltage )) ) };
	COLORREF const color   { RGB( colElem, 0, 0 ) };
	return color;
}

float Shape::GetFillLevel( mV const voltageInput ) const
{
	return voltageInput.GetValue() / m_pNNetModel->GetParameterValue( tParameter::threshold );
}

void Shape::CheckInputBuffer( ) const
{
//	assert( m_mVinputBuffer <= mV( m_pNNetModel->GetParameterValue( tParameter::peakVoltage ) * 2 ) );
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

tShapeType const Shape::GetShapeTypeFromName( wchar_t const * const name )
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

bool IsPipelineType( tShapeType const type )
{
	return type == tShapeType::pipeline;
}

bool IsKnotType( tShapeType const type )
{
	return type == tShapeType::knot;
}

bool IsNeuronType( tShapeType const type )
{
	return 
		(type == tShapeType::neuron)      || 
		(type == tShapeType::inputNeuron);
}

bool IsBaseKnotType( tShapeType const type )
{
	return 
		(type == tShapeType::knot)        || 
		(type == tShapeType::neuron)      || 
		(type == tShapeType::inputNeuron);
}
