// Neuron.cpp 
//
// NNetModel

module;

#include <cassert> 
#include <sstream> 
#include <iomanip>
#include "Resource.h"

module NNetModel:Neuron;

import Types;
import DrawContext;
import :NNetColors;
import :Spike;
import :NNetParameters;

using std::fixed;
using std::wstring;
using std::wostringstream;

Neuron::Neuron(MicroMeterPnt const& upCenter)
	: PosNob(NobType::Value::neuron),
	m_circle(upCenter, NEURON_RADIUS)
{}

//Neuron::Neuron(Neuron const& rhs)
//  : PosNob(NobType::Value::neuron),
//	m_circle(rhs.m_circle)
//{
//	PosNob::operator=(rhs);
//	init(rhs);
//}

void Neuron::init(const Neuron & rhs)
{
	m_bStopOnTrigger = rhs.m_bStopOnTrigger;
	m_usSpikeTime    = rhs.m_usSpikeTime;
	m_bTriggered     = rhs.m_bTriggered;
	m_inPipes        = rhs.m_inPipes;
	m_pPipeAxon      = rhs.m_pPipeAxon;
}

//Neuron & Neuron::operator=(Neuron const& rhs)
//{
//	PosNob::operator=(rhs);
//	init(rhs);
//	return * this;
//}

void Neuron::SetPosNoFix(MicroMeterPnt const& newPos)
{
	m_circle.SetPos(newPos);
}

void Neuron::Recalc()
{
	m_pPipeAxon->PosChanged();
	m_inPipes.Apply2All([](Pipe& pipe) { pipe.PosChanged(); });
}

void Neuron::ClearDynamicData()
{
	PosNob::ClearDynamicData();
	m_usSpikeTime = 0.0_MicroSecs;
	m_bTriggered = false;
}

void Neuron::CollectInput()
{
	if (m_bTriggered)
	{
		if (m_usSpikeTime > GetParam()->PulseDistMin())
			m_bTriggered = false;
	}
	else 
	{
		m_mVpotential.Set2Zero();
		Apply2AllInPipesC([this](Pipe const& pipe) { m_mVpotential += pipe.GetPotential(); });
	}
}

bool Neuron::CompStep()
{
	bool bTrigger { m_mVpotential >= GetParam()->Threshold() };

	if (bTrigger)
	{
		m_usSpikeTime = 0.0_MicroSecs;
		m_bTriggered  = true;
		m_mVpotential.Set2Zero();
	}
	else
	{
		m_usSpikeTime += GetParam()->TimeResolution();
	}

	return m_bStopOnTrigger && bTrigger;
}

mV Neuron::GetPotential() const   // TODO: Compute in CompStep?
{
	mV         const amplitude  { GetParam()->NeuronPeakVolt() };
	fMicroSecs const pulseWidth { GetParam()->PulseWidth() };
	return Spike::GetVoltage(amplitude, pulseWidth, m_usSpikeTime);
}

MicroMeterPnt Neuron::getAxonHillockPos() const
{
	float         const fRatio       { GetExtension() / m_pPipeAxon->GetLength() };
	MicroMeterPnt const vectorScaled { m_pPipeAxon->GetVector() * fRatio };
	return GetPos() + vectorScaled * NEURON_INTERIOR;
//	return GetPos() + Normalize(m_pPipeAxon->GetVector()) * (NEURON_INTERIOR * NEURON_RADIUS);  //TODO: try this
}

void Neuron::AddIncoming(Neuron const * pNeuron)
{
	pNeuron->m_inPipes.Apply2All([this](Pipe& pipe) { AddIncoming(&pipe); });
}

void Neuron::RemoveIncoming(Pipe* const pDel)
{
	m_inPipes.Remove(*pDel);
}

void Neuron::ReplaceIncoming(Pipe* const pDel, Pipe* const pAdd)
{
	m_inPipes.Replace(pDel, pAdd);
}

void Neuron::ReplaceOutgoing(Pipe* const pDel, Pipe* const pAdd)
{
	if (pDel == m_pPipeAxon)
		m_pPipeAxon = pAdd;
	else
		assert(false);
}

void Neuron::DrawExterior(DrawContext const & context, tHighlight const type) const
{
	if (m_bStopOnTrigger)
	{
		context.FillCircle(m_circle * 1.4f, GetExteriorColor(type));
		context.FillCircle(m_circle * 1.2f, NNetColors::INT_TRIGGER);
	}
	context.FillCircle(m_circle, GetExteriorColor(type));
	context.FillCircle(MicroMeterCircle(getAxonHillockPos(), GetExtension() * 0.5f), GetExteriorColor(type));
}

void Neuron::DrawInterior(DrawContext const & context, tHighlight const type) const
{ 
	D2D1::ColorF const color
	{
		(m_bTriggered && (m_usSpikeTime < GetParam()->PulseWidth()))
		? NNetColors::INT_TRIGGER
		: Nob::GetInteriorColor(type, m_mVpotential)
	};
	context.FillCircle(m_circle * NEURON_INTERIOR, color);
	context.FillCircle(MicroMeterCircle(getAxonHillockPos(), GetExtension() * (NEURON_INTERIOR - 0.5f)), color);
}

void Neuron::AppendMenuItems(AddMenuFunc const & add) const
{
	add(IDD_ADD_INCOMING2NEURON);  // case 9
	if (GetNrOfInConns() == 1)
		add(IDD_SPLIT_NEURON);
	add(IDD_STOP_ON_TRIGGER);
	add(IDD_DELETE_NOB);
	PosNob::AppendMenuItems(add);
}

void Neuron::SelectAllConnected(bool const bFirst)
{
	if (!IsSelected() || bFirst)
	{
		Nob::Select(true);
		m_inPipes.SelectAllConnected();
		m_pPipeAxon->SelectAllConnected(false);
	}
}

void Neuron::MoveNob(MicroMeterPnt const& delta)
{
	SetPos(GetPos() + delta);
}

void Neuron::Link(Nob const& nobSrc, Nob2NobFunc const& f)
{
	Neuron const& src { *Cast2Neuron(&nobSrc) };
	m_pPipeAxon = static_cast<Pipe*>(f(src.m_pPipeAxon));
	m_inPipes.Clear();
	src.Apply2AllInPipesC([this, f](Pipe const& p) { m_inPipes.Add(static_cast<Pipe&>(*f(&p))); });
}

void Neuron::RotateNob(MicroMeterPnt const& umPntPivot, Radian const radDelta)
{
	m_circle.Rotate(umPntPivot, radDelta);
}

void Neuron::Reconnect()
{
	m_inPipes.Apply2All
	(
		[this](Pipe& pipe) 
		{ 
			pipe.SetEndPnt(this); 
			pipe.PosChanged();
		}
	);
	m_pPipeAxon->SetStartPnt(this);
	m_pPipeAxon->PosChanged();
}

void Neuron::SetAllIncoming(PosNob& src) 
{ 
	m_inPipes = Cast2Neuron(&src)->m_inPipes; 
}

void Neuron::SetAllOutgoing(PosNob& src) 
{ 
	m_pPipeAxon = Cast2Neuron(&src)->m_pPipeAxon; 
}
