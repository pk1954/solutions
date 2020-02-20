// Shape.cpp 
//
// NNetModel

#include "stdafx.h"
#include "NNetModel.h"
#include "shape.h"

D2D_driver const * Shape::m_pGraphics  { nullptr };
NNetModel  const * Shape::m_pNNetModel { nullptr };

D2D1::ColorF Shape::GetInteriorColor( mV const voltageInput ) const
{
	if ( m_bEmphasized )
		return EXT_COLOR_SUPER_HIGHLIGHT;

	mV    const peakVoltage { mV(m_pNNetModel->GetParameterValue( tParameter::peakVoltage )) };
	float const colElem     { min( voltageInput / peakVoltage, 1.0f )};
	float const fOpacity    { m_pNNetModel->GetOpacity() };
	return D2D1::ColorF( colElem, 0.0f, 0.0f, fOpacity );
}

float Shape::GetFillLevel( mV const voltageInput ) const
{
	return voltageInput.GetValue() / m_pNNetModel->GetParameterValue( tParameter::threshold );
}
