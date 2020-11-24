// Shape.cpp 
//
// NNetModel

#include "stdafx.h"
#include "scanner.h"
#include "NNetColors.h"
#include "shape.h"
#include "debug.h"

using std::move;
using std::wcout;
using std::wostream;

Shape::Shape( ShapeType const type )
	: m_type( type )
{ }	

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

void Shape::Dump( ) const
{
	wcout << Scanner::COMMENT_SYMBOL << *this;
}

wostream & operator<< ( wostream & out, Shape const & shape )
{
	out << shape.m_identifier << L' ' << shape.m_type;
	return out;
}
