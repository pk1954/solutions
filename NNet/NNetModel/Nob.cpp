// Nob.cpp 
//
// NNetModel

module;

#include <cassert> 

module NNetModel:Nob;

import std;
import Win32_Util_Resource;
import Debug;
import Types;
import ColorLUT;
import IoConstants;
import IoUtil;
import :tHighlight;
import :NNetColors;
import :NNetParameters;
import :NobType;

using std::min;
using std::setw;
using std::move;
using std::wcout;
using std::wostream;
using std::make_unique;

Nob::Nob(Nob const & rhs)
{
	m_mVpotential = rhs.m_mVpotential;
	m_identifier  = rhs.m_identifier;
	m_states      = rhs.m_states;
}

bool Nob::operator==(Nob const & rhs) const
{
    if (GetNobType() != rhs.GetNobType())
		 return false;
    if (m_states != rhs.m_states)
		 return false;
	return true;
}

void Nob::SetPos(MicroMeterPnt const& umPos)
{
	SetPosNoFix(umPos);
	Recalc();
}

Color Nob::GetInteriorColor(mV const voltage) const
{
	mV    const threshold { mV(GetParam()->GetParameterValue(ParamType::Value::threshold)) };
	float const fFactor   { min(voltage / threshold, 1.0f)};
	if (IsEmphasized() && (fFactor < 0.01f))
		return NNetColors::INT_EMPHASIZED;
	else
	{
		ColIndex const colIndex { Cast2Byte(255.0f * fFactor) };
		Color          color    { m_pLutVoltage->GetColor(colIndex) };
		if (IsSelected())
			color.a = 0.7f;
		return color;
	}
}

Color Nob::GetExteriorColor(tHighlight const type) const
{
	if (type == tHighlight::highlighted) 
		return NNetColors::EXT_HIGHLIGHTED;
	else 
		return IsEmphasized() ? NNetColors::EXT_EMPHASIZED : NNetColors::EXT_NORMAL;
};

Color Nob::GetInteriorColor(tHighlight const type, mV const voltage) const
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
#endif
}

void Nob::Dump() const
{
	wcout << COMMENT_SYMBOL;
	wcout << (IsSelected() ? L'S' : SPACE);
	wcout << *this;
}

wostream & operator<< (wostream & out, Nob const & nob)
{
	out << setw(5) << nob.m_identifier << SPACE << nob.GetNobType();
	if (nob.HasParentNob())
		out << L" (par" << nob.GetParentNob()->m_identifier << ")";
	return out;
}

void Nob::AppendMenuItems(AddMenuFunc const & add) const
{
	add(IDM_SELECT);
}
