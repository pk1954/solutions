// Nob.cpp 
//
// NNetModel

module;

#include <cassert> 
#include <memory>
#include <iomanip>
#include <iostream>
#include <algorithm>
#include "Resource.h"

module NNetModel:Nob;

import Debug;
import Types;
import Scanner;
import :tHighlight;
import :NNetColors;
import :NNetParameters;
import :MicroMeterPosDir;
import :NobType;

using std::min;
using std::setw;
using std::move;
using std::wcout;
using std::wostream;
using std::make_unique;

Nob::Nob(NobType const type)
	: m_type(type)
{}	

Nob::Nob(Nob const & rhs)
{
	m_mVpotential   = rhs.m_mVpotential;
	m_type          = rhs.m_type;
	m_identifier    = rhs.m_identifier;
	m_bSelected     = rhs.m_bSelected;
	m_bEmphasized   = rhs.m_bEmphasized;
	if (rhs.HasMicroSensor())
		CreateMicroSensor();
}

bool Nob::operator==(Nob const & rhs) const
{
    if (m_type != rhs.m_type)
		 return false;
	if (m_identifier != rhs.m_identifier)
		return false;
	if (m_bSelected != rhs.m_bSelected)
		return false;
	return true;
}

void Nob::SetPos(MicroMeterPnt const& umPos)
{
	SetPosNoFix(umPos);
	Recalc();
}

void Nob::SetDir(Radian const radian)
{ 
	RotateNob(GetPos(), radian - GetDir()); 
};

void Nob::SetPosDir(MicroMeterPosDir const & umPosDir) 
{
	SetDir(umPosDir.GetDir());
	SetPos(umPosDir.GetPos());
};

ColorF Nob::GetInteriorColor(mV const voltage) const
{
	mV    const threshold      { mV(GetParam()->GetParameterValue(ParamType::Value::threshold)) };
	float const colorComponent { min(voltage / threshold, 1.0f)};
	float const fAlphaChannel  { m_bSelected ? 0.7f : 1.0f };
	if (IsEmphasized() && (colorComponent < 0.01f))
		return NNetColors::INT_EMPHASIZED;
	else 
		return ColorF(colorComponent, 0.0f, 0.0f, fAlphaChannel);
}

ColorF Nob::GetExteriorColor(tHighlight const type) const
{
	if (type == tHighlight::highlighted) 
		return NNetColors::EXT_HIGHLIGHTED;
	else 
		return IsEmphasized() ? NNetColors::EXT_EMPHASIZED : NNetColors::EXT_NORMAL;
};

ColorF Nob::GetInteriorColor(tHighlight const type, mV const voltage) const
{
	Nob const * pNob { this };
	while (pNob->HasParentNob())
		pNob = pNob->GetParentNob();
	if (IsEmphasized())
		switch (type)
		{
		case tHighlight::normal:      return pNob->IsSelected() ? NNetColors::INT_EMP_SELECTED : NNetColors::INT_EMPHASIZED;
		case tHighlight::highlighted: return pNob->IsSelected() ? NNetColors::INT_EMP_SELECTED : NNetColors::INT_EMPHASIZED;
		case tHighlight::targetFit:   return NNetColors::INT_TARGET_FIT;
		case tHighlight::targetNoFit: return NNetColors::INT_TARGET_NOFIT;
		default: assert(false);
		}
	else
		switch (type)
		{
		case tHighlight::normal:      return pNob->IsSelected() ? NNetColors::INT_SELECTED : GetInteriorColor(voltage);
		case tHighlight::highlighted: return pNob->IsSelected() ? NNetColors::INT_SELECTED : NNetColors::INT_NORMAL;
		case tHighlight::targetFit:   return NNetColors::INT_TARGET_FIT;
		case tHighlight::targetNoFit: return NNetColors::INT_TARGET_NOFIT;
		default: assert(false);
		}
	return NNetColors::INT_NORMAL;
};

void Nob::Check() const
{
#ifdef _DEBUG
	m_type.Check();
	AssertLimits<int>((int)m_bSelected, 0, 1);
#endif
}

void Nob::Dump() const
{
	wcout << Scanner::COMMENT_SYMBOL;
	wcout << (IsSelected() ? L'S' : L' ');
	wcout << *this;
}

wostream & operator<< (wostream & out, Nob const & nob)
{
	out << setw(5) << nob.m_identifier.GetValue() << L' ' << nob.m_type;
	if (nob.HasParentNob())
		out << L" (par " << nob.GetParentNob()->m_identifier.GetValue() << ")";
	return out;
}

void Nob::AppendMenuItems(AddMenuFunc const & add) const
{
	add(IDM_SELECT);
}

MicroSensor* Nob::CreateMicroSensor()
{
	m_upMicroSensor = make_unique<MicroSensor>(this);
	return m_upMicroSensor.get();
};
