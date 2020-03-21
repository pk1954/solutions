// Shape.cpp 
//
// NNetModel

#include "stdafx.h"
#include "synchapi.h"
#include "NNetModel.h"
#include "shape.h"

D2D_driver const * Shape::m_pGraphics  { nullptr };
NNetModel  const * Shape::m_pNNetModel { nullptr };

Shape::Shape( ShapeType const type )
  : m_mVinputBuffer( 0._mV ),
	m_bEmphasized( false ),
	m_identifier( NO_SHAPE ),
	m_type( type )
{ 
	(void)InitializeCriticalSectionAndSpinCount( & m_criticalSection, 0x00000400 );
}

Shape::~Shape() 
{
	DeleteCriticalSection( & m_criticalSection );
}

D2D1::ColorF Shape::GetInteriorColor( mV const voltageInput ) const
{
	if ( m_bEmphasized )
	{
		return INT_COLOR_EMPHASIZED;
	}
	else if ( m_pNNetModel->IsInEmphasizeMode( ) )
		return INT_COLOR_LOW_KEY;
	else
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
		if ( ! m_pNNetModel->IsInEmphasizeMode( ) || m_bEmphasized )
			return EXT_COLOR_NORMAL;
		else
			return EXT_COLOR_LOW_KEY;
	}

	if (type == tHighlightType::highlighted)
		return EXT_COLOR_HIGHLIGHT;
	else 
		return EXT_COLOR_SUPER_HIGHLIGHT;
};

float Shape::GetFillLevel( mV const voltageInput ) const
{
	return voltageInput.GetValue() / m_pNNetModel->GetParameterValue( tParameter::threshold );
}
