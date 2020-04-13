// Shape.cpp 
//
// NNetModel

#include "stdafx.h"
#include "synchapi.h"
#include "NNetModel.h"
#include "NNetColors.h"
#include "shape.h"

D2D_driver const * Shape::m_pGraphics  { nullptr };
NNetModel  const * Shape::m_pNNetModel { nullptr };

Shape::Shape( ShapeType const type )
  : m_type( type )
{ }			

D2D1::ColorF Shape::GetInteriorColor( mV const voltageInput ) const
{
	if ( m_pNNetModel->IsInEmphasizeMode( ) && ! m_bSelected )
	{
		return NNetColors::INT_LOW_KEY;
	}
	else if ( m_bSelected )
	{
		return NNetColors::m_colSelected;
	}
	else  // normal mode
	{
		mV    const peakVoltage  { mV(m_pNNetModel->GetParameterValue( tParameter::peakVoltage )) };
		float const redComponent { min( voltageInput / peakVoltage, 1.0f )};
		return D2D1::ColorF( redComponent, 0.0f, 0.0f, 1.0f );
	}
}

D2D1::ColorF Shape::GetFrameColor( tHighlightType const type ) const 
{ 
	if (type == tHighlightType::normal)
	{
		if ( m_pNNetModel->IsInEmphasizeMode( ) && ! m_bSelected )
			return NNetColors::EXT_LOW_KEY;
		else
			return NNetColors::EXT_NORMAL;
	}
	else if (type == tHighlightType::highlighted)
	{
		return NNetColors::EXT_HIGHLIGHT;
	}
	else 
	{
		return NNetColors::EXT_SUPER_HIGHLIGHT;
	}
};

float Shape::GetFillLevel( mV const voltageInput ) const
{
	return voltageInput.GetValue() / m_pNNetModel->GetParameterValue( tParameter::threshold );
}
