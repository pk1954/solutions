// Nob.cpp 
//
// NNetModel

#include "stdafx.h"
#include "Resource.h"
#include "SCANNER.H"
#include "NNetColors.h"
#include "Nob.h"
#include "debug.h"

using std::move;
using std::wcout;
using std::wostream;

Nob::Nob(NobType const type)
	: m_type(type)
{}	

Nob::Nob(Nob const & src)
{
	*this = src;
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

MicroMeterPosDir Nob::GetPosDir() const 
{ 
	return MicroMeterPosDir(GetPos(), GetDir()); 
};

ColorF Nob::GetInteriorColor(mV const voltage) const
{
	mV const threshold { mV(m_pParameters->GetParameterValue(ParamType::Value::threshold)) };
	if (IsAnyNeuron() && (voltage > threshold * 1.3f))
	{
		int x = 42;
	}
	float const colorComponent { min(voltage / threshold, 1.0f)};
	float const fAlphaChannel  { m_bSelected ? 0.7f : 1.0f };
	if (IsEmphasized() &&  (colorComponent < 0.01f))
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

ColorF Nob::GetInteriorColor(tHighlight const type) const 
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
		case tHighlight::normal:      return pNob->IsSelected() ? NNetColors::INT_SELECTED : GetInteriorColor();
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
	out << setw(5) << nob.m_identifier << L' ' << nob.m_type;
	if (nob.m_pNobParent)
		out << L" (par " << nob.m_pNobParent->m_identifier << ")";
	return out;
}

MicroMeterPosDir CalcOffsetPosDir(Nob const & nob, MicroMeter const umO)
{
	MicroMeter    const umOff        { umO * NEURON_RADIUS.GetValue() };
	MicroMeter    const umOffset     { (nob.GetIoMode() == NobIoMode::input) ? -umOff : umOff };
	Radian        const radianTarget { nob.GetDir() };
	MicroMeterPnt const umDirVector  { Normalize(Radian2Vector(radianTarget)) };
	MicroMeterPnt const umPosOffset  { umDirVector * umOffset.GetValue() };
	MicroMeterPnt const umPosTarget  { nob.GetPos() + umPosOffset };
	return MicroMeterPosDir(umPosTarget, radianTarget);
}

void Nob::AppendMenuItems(AddMenuFunc const & add) const
{
	if (IsSelected())
		add(IDM_DESELECT_NOB);
	else
		add(IDM_SELECT_NOB);
	add(IDD_DELETE_NOB);
}
