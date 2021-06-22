// Nob.cpp 
//
// NNetModel

#include "stdafx.h"
#include "scanner.h"
#include "NNetColors.h"
#include "nob.h"
#include "debug.h"

using std::move;
using std::wcout;
using std::wostream;

Nob::Nob( NobType const type )
	: m_type( type )
{ }	

bool Nob::operator==( Nob const & rhs ) const
{
	return
	( m_type       == rhs.m_type       ) &&
	( m_identifier == rhs.m_identifier ) &&
	( m_bSelected  == rhs.m_bSelected  );
}

void Nob::SetDir(Radian const radian) 
{ 
	RotateNob(GetPos(), radian - GetDir()); 
};

void Nob::SetPos(MicroMeterPnt const & umPosNew) 
{ 
	MoveNob(umPosNew - GetPos()); 
}

void Nob::SetPosDir(MicroMeterPosDir const & umPosDir) 
{
	SetDir(umPosDir.GetDir());
	SetPos(umPosDir.GetPos());
};

MicroMeterPosDir const Nob::GetPosDir() const 
{ 
	return MicroMeterPosDir(GetPos(), GetDir()); 
};

D2D1::ColorF Nob::GetInteriorColor( mV const voltageInput ) const
{
	mV    const peakVoltage    { mV(m_pParameters->GetParameterValue( ParamType::Value::peakVoltage )) };
	float const colorComponent { min( voltageInput / peakVoltage, 1.0f )};
	float const fAlphaChannel  { m_bSelected ? 0.7f : 1.0f };
	return D2D1::ColorF( colorComponent, 0.0f, 0.0f, fAlphaChannel );
}

D2D1::ColorF Nob::GetExteriorColor( tHighlight const type ) const 
{
	switch ( type )
	{
	case tHighlight::highlighted: 
		return NNetColors::EXT_HIGHLIGHTED;
	case tHighlight::normal:      
	case tHighlight::targetFit:   
	case tHighlight::targetNoFit: 
		return HasParentNob() ? NNetColors::EXT_CONNECTOR : NNetColors::EXT_NORMAL;
	}
	assert( false );
	return NNetColors::EXT_NORMAL;
};

D2D1::ColorF Nob::GetInteriorColor( tHighlight const type ) const 
{
	Nob const * pNob { this };
	while ( pNob->HasParentNob() )
		pNob = pNob->GetParentNob();
	switch ( type )
	{
	case tHighlight::normal:      return pNob->IsSelected() ? NNetColors::INT_SELECTED : GetInteriorColor();
	case tHighlight::highlighted: return pNob->IsSelected() ? NNetColors::INT_SELECTED : NNetColors::INT_NORMAL;
	case tHighlight::targetFit:   return NNetColors::INT_TARGET_FIT;
	case tHighlight::targetNoFit: return NNetColors::INT_TARGET_NOFIT;
	}
	assert( false );
	return NNetColors::INT_NORMAL;
};

float Nob::GetFillLevel( mV const voltageInput ) const
{
	return voltageInput.GetValue() / m_pParameters->GetParameterValue( ParamType::Value::threshold );
}

void Nob::Check() const
{
#ifdef _DEBUG
	m_type.Check();
	AssertLimits<int>( (int)m_bSelected, 0, 1 );
#endif
}

void Nob::Dump() const
{
	wcout << Scanner::COMMENT_SYMBOL;
	wcout << (IsSelected() ? L'S' : L' ');
	wcout << *this;
}

wostream & operator<< ( wostream & out, Nob const & nob )
{
	out << setw(5) << nob.m_identifier << L' ' << nob.m_type;
	if (nob.m_pNobParent)
		out << L" (par " << nob.m_pNobParent->m_identifier << ")";
	return out;
}

MicroMeterPosDir const CalcOffsetPosDir(Nob const & nob, MicroMeter const umO)
{
	MicroMeter    const umOff        { umO * NEURON_RADIUS.GetValue() };
	MicroMeter    const umOffset     { (nob.GetIoMode() == NobIoMode::input) ? -umOff : umOff };
	Radian        const radianTarget { nob.GetDir() };
	MicroMeterPnt const umDirVector  { Normalize(Radian2Vector(radianTarget)) };
	MicroMeterPnt const umPosOffset  { umDirVector * umOffset.GetValue() };
	MicroMeterPnt const umPosTarget  { nob.GetPos() + umPosOffset };
	return MicroMeterPosDir(umPosTarget, radianTarget);
}