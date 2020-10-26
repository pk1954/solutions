// Shape.cpp 
//
// NNetModel

#include "stdafx.h"
#include "NNetColors.h"
#include "shape.h"
#include "debug.h"

using std::move;

Shape::Shape( ShapeType const type )
	: m_type( type )
{ }	

Shape::Shape( Shape const & src ) :  // copy constructor
  	m_type      (src.m_type),
	m_identifier(src.m_identifier),
	m_bSelected (src.m_bSelected),
	m_bMarked   (src.m_bMarked)
{ }

Shape::Shape( Shape&& rhs ) noexcept
{
	*this = move( rhs );
}

Shape & Shape::operator= ( Shape&& rhs ) noexcept
{
	if (this != &rhs)
	{
		m_type       = rhs.m_type;
		m_identifier = rhs.m_identifier;
		m_bSelected  = rhs.m_bSelected;  
		m_bMarked    = rhs.m_bMarked;

		rhs.m_type       = ShapeType::Value::undefined;
		rhs.m_identifier = NO_SHAPE;
		rhs.m_bSelected  = false;
		rhs.m_bMarked    = false;
	}
	return *this;
}

bool Shape::operator==( Shape const & rhs ) const
{
	return
	( m_type       == rhs.m_type       ) &&
	( m_identifier == rhs.m_identifier ) &&
	( m_bSelected  == rhs.m_bSelected  ) &&
	( m_bMarked    == rhs.m_bMarked    );
}

D2D1::ColorF Shape::GetInteriorColor( mV const voltageInput ) const
{
	if ( m_bSelected )
	{
		return NNetColors::m_colSelected;
	}
	else  // normal mode
	{
		mV    const peakVoltage    { mV(m_pParameters->GetParameterValue( tParameter::peakVoltage )) };
		float const colorComponent { min( voltageInput / peakVoltage, 1.0f )};
		return m_bMarked 
			? D2D1::ColorF( colorComponent, 0.6f, 0.6f, 1.0f )
		    : D2D1::ColorF( colorComponent, 0.0f, 0.0f, 1.0f );
	}
}

D2D1::ColorF Shape::GetFrameColor( tHighlightType const type ) const 
{ 
	if (type == tHighlightType::normal)
	{
		return m_bMarked ? NNetColors::EXT_MARKED : NNetColors::EXT_NORMAL;
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
	return voltageInput.GetValue() / m_pParameters->GetParameterValue( tParameter::threshold );
}

void Shape::CheckShape( ) const
{
#ifdef _DEBUG
	m_type.Check();
	AssertLimits<int>( (int)m_bSelected, 0, 1 );
	AssertLimits<int>( (int)m_bMarked,   0, 1 );
#endif
}