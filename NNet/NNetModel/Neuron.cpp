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

Neuron::Neuron(MicroMeterPnt const & upCenter)
  : BaseKnot(upCenter, NobType::Value::neuron, NEURON_RADIUS)
{}

Neuron::Neuron(Neuron const & src)  // copy constructor
  : BaseKnot(src)
{ 
	init(src);
}

void Neuron::init(const Neuron & rhs)
{
	m_bStopOnTrigger = rhs.m_bStopOnTrigger;
	m_usSpikeTime    = rhs.m_usSpikeTime;
	m_bTriggered     = rhs.m_bTriggered;
}

Neuron & Neuron::operator=(Neuron const & rhs)
{
	BaseKnot::operator=(rhs);
	init(rhs);
	return * this;
}

void Neuron::ClearDynamicData()
{
	BaseKnot::ClearDynamicData();
	m_usSpikeTime = 0.0_MicroSecs;
	m_bTriggered = false;
}

void Neuron::CollectInput()
{
	if (m_bTriggered)
	{
		if (m_usSpikeTime > GetParam()->SpikeWidth() + GetParam()->RefractPeriod())
			m_bTriggered = false;
	}
	else 
	{
		m_mVinputBuffer.Set2Zero();
		Apply2AllInPipes([this](Pipe const& pipe) { m_mVinputBuffer += pipe.GetNextOutput(); }); // slow !!
	}
}

bool Neuron::CompStep()
{
	bool bTrigger { m_mVinputBuffer >= GetParam()->NeuronThreshold() };

	if (bTrigger)
	{
		m_usSpikeTime = 0.0_MicroSecs;
		m_bTriggered  = true;
		m_mVinputBuffer.Set2Zero();
	}
	else
	{
		m_usSpikeTime += GetParam()->TimeResolution();
	}

	return m_bStopOnTrigger && bTrigger;
}

mV Neuron::GetNextOutput() const
{
	mV         const amplitude  { GetParam()->NeuronPeakVolt() };
	fMicroSecs const spikeWidth { GetParam()->SpikeWidth() };
	return Spike::GetVoltage(amplitude, spikeWidth, m_usSpikeTime);
}

void Neuron::DisplayText(DrawContext const & context, MicroMeterRect const & umRect, wstring const & text) const
{
	MicroMeterPnt const umPosHalfHeight { 0._MicroMeter, umRect.GetHeight()/2 };
	context.DisplayText(umRect + umPosHalfHeight, text, D2D1::ColorF::GreenYellow);
}

MicroMeterPnt Neuron::getAxonHillockPos() const
{
	Pipe          const & axon         { GetFirstOutgoing() };
	MicroMeterPnt const   vectorScaled { axon.GetVector() * (GetExtension() / axon.GetLength()) };
	return GetPos() + vectorScaled * NEURON_INTERIOR;
}

void Neuron::DrawExterior(DrawContext const & context, tHighlight const type) const
{
	if (m_bStopOnTrigger)
	{
		context.FillCircle(GetCircle() * 1.4f, GetExteriorColor(type));
		context.FillCircle(GetCircle() * 1.2f, NNetColors::INT_TRIGGER);
	}
	context.FillCircle(GetCircle(), GetExteriorColor(type));
	if (HasAxon())
		context.FillCircle(MicroMeterCircle(getAxonHillockPos(), GetExtension() * 0.5f), GetExteriorColor(type));
}

void Neuron::DrawInterior(DrawContext const & context, tHighlight const type) const
{ 
	D2D1::ColorF const color { m_bTriggered ? NNetColors::INT_TRIGGER : Nob::GetInteriorColor(type) };
	context.FillCircle(GetCircle() * NEURON_INTERIOR, color);
	if (HasAxon())
		context.FillCircle(MicroMeterCircle(getAxonHillockPos(), GetExtension() * (NEURON_INTERIOR - 0.5f)), color);
}

void Neuron::AppendMenuItems(AddMenuFunc const & add) const
{
	add(IDD_ADD_INCOMING2BASEKNOT);
	add(IDD_SPLIT_NEURON);
	add(IDD_STOP_ON_TRIGGER);
	BaseKnot::AppendMenuItems(add);
}
