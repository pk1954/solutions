// Shape.cpp 
//
// NNetModel

#include "stdafx.h"
#include "NNetModel.h"
#include "shape.h"

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
