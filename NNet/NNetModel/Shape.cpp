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
	( m_bSelected  == rhs.m_bSelected  );
}

D2D1::ColorF Shape::GetInteriorColor( mV const voltageInput ) const
{
	mV    const peakVoltage    { mV(m_pParameters->GetParameterValue( ParamType::Value::peakVoltage )) };
	float const colorComponent { min( voltageInput / peakVoltage, 1.0f )};
	float const fAlphaChannel  { m_bSelected ? 0.7f : 1.0f };
	return D2D1::ColorF( colorComponent, 0.0f, 0.0f, fAlphaChannel );
}

D2D1::ColorF Shape::GetExteriorColor( tHighlight const type ) const 
{
	switch ( type )
	{
	case tHighlight::normal:      return NNetColors::EXT_NORMAL;
	case tHighlight::highlighted: return NNetColors::EXT_HIGHLIGHTED;
	case tHighlight::targetFit:   return NNetColors::EXT_NORMAL;
	case tHighlight::targetNoFit: return NNetColors::EXT_NORMAL;
	}
	assert( false );
	return NNetColors::EXT_NORMAL;
};

D2D1::ColorF Shape::GetInteriorColor( tHighlight const type ) const 
{ 
	switch ( type )
	{
	case tHighlight::normal:      return IsSelected() ? NNetColors::INT_SELECTED : GetInteriorColor();
	case tHighlight::highlighted: return IsSelected() ? NNetColors::INT_SELECTED : NNetColors::INT_NORMAL;
	case tHighlight::targetFit:   return NNetColors::INT_TARGET_FIT;
	case tHighlight::targetNoFit: return NNetColors::INT_TARGET_NOFIT;
	}
	assert( false );
	return NNetColors::INT_NORMAL;
};

float Shape::GetFillLevel( mV const voltageInput ) const
{
	return voltageInput.GetValue() / m_pParameters->GetParameterValue( ParamType::Value::threshold );
}

void Shape::CheckShape() const
{
#ifdef _DEBUG
	m_type.Check();
	AssertLimits<int>( (int)m_bSelected, 0, 1 );
#endif
}

void Shape::Dump() const
{
	wcout << Scanner::COMMENT_SYMBOL << *this;
}

wostream & operator<< ( wostream & out, Shape const & shape )
{
	out << shape.m_identifier << L' ' << shape.m_type;
	return out;
}
