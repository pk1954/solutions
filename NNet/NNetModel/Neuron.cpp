// Neuron.cpp 
//
// NNetModel

#include "stdafx.h"
#include <sstream> 
#include <iomanip>
#include "Resource.h"
#include "win32_sound.h"
#include "win32_thread.h"
#include "win32_graphicsInterface.h"
#include "win32_beeperThread.h"
#include "DrawContext.h"
#include "NNetParameters.h"
#include "NNetColors.h"
#include "Spike.h"
#include "IoLine.h"
#include "Pipe.h"
#include "Neuron.h"

using std::fixed;
using std::wstring;
using std::wostringstream;

Neuron::Neuron(MicroMeterPnt const & upCenter, NobType const type)
  : BaseKnot(upCenter, type, NEURON_RADIUS)
{}

Neuron::Neuron(Neuron const & src)  // copy constructor
	: BaseKnot(src)
{ 
	init(src);
}

Neuron::Neuron(BaseKnot const & src, NobType const type)
	: BaseKnot(src)
{
	SetType(type);
	SetExtension(NEURON_RADIUS);
}

static void CALLBACK BeepFunc(PTP_CALLBACK_INSTANCE, PVOID arg, PTP_WORK)
{
	Neuron const * pNeuron { static_cast<Neuron const *>(arg) };
	Neuron::m_pSound->Beep(pNeuron->GetTriggerSound());
}

void Neuron::init(const Neuron & rhs)
{
	m_bStopOnTrigger = rhs.m_bStopOnTrigger;
	m_usSpikeTime    = rhs.m_usSpikeTime;
	m_bTriggered     = rhs.m_bTriggered;
	m_triggerSound   = rhs.m_triggerSound;
	m_pTpWork = (rhs.m_triggerSound.m_bOn )
		      ? CreateThreadpoolWork(BeepFunc, this, nullptr)
		      : nullptr;
}

Neuron & Neuron::operator=(Neuron const & rhs)
{
	BaseKnot::operator=(rhs);
	init(rhs);
	return * this;
}

bool Neuron::operator==(Nob const & rhs) const
{
	Neuron const & neuronRhs { static_cast<Neuron const &>(rhs) };
	return
	(this->BaseKnot::operator== (neuronRhs))                             &&
	(m_triggerSound.m_bOn       == neuronRhs.m_triggerSound.m_bOn)       &&
	(m_triggerSound.m_frequency == neuronRhs.m_triggerSound.m_frequency) &&
	(m_triggerSound.m_duration  == neuronRhs.m_triggerSound.m_duration);
}

SoundDescr Neuron::SetTriggerSound(SoundDescr const & sound) 
{
	SoundDescr oldValue { m_triggerSound };
	if (m_triggerSound.m_bOn != sound.m_bOn)
	{
		if (sound.m_bOn )
			m_pTpWork = CreateThreadpoolWork(BeepFunc, this, nullptr);
		else
		{
			CloseThreadpoolWork(m_pTpWork);
			m_pTpWork = nullptr;
		}
	}
	m_triggerSound = sound;
	return oldValue;
}

void Neuron::ClearDynamicData()
{
	BaseKnot::ClearDynamicData();
	m_usSpikeTime = 0.0_MicroSecs;
	m_bTriggered = false;
}

void Neuron::Prepare()
{
	if (m_bTriggered)
	{
		if (m_usSpikeTime > GetParam()->SpikeWidth() + GetParam()->RefractPeriod())
			m_bTriggered = false;
	}
	else 
	{
		BaseKnot::Prepare();
	}
}

bool Neuron::CompStep()
{
	bool bTrigger { m_mVinputBuffer >= GetParam()->Threshold() };

	if (bTrigger)
	{
		m_usSpikeTime = 0.0_MicroSecs;
		m_bTriggered = true;
		m_mVinputBuffer.Set2Zero();
		if (HasTriggerSound() && m_pTpWork)
			SubmitThreadpoolWork(m_pTpWork);
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

Neuron const * Cast2Neuron(Nob const * pNob)
{
	assert(pNob->IsAnyNeuron());
	return static_cast<Neuron const *>(pNob);
}

Neuron * Cast2Neuron(Nob * pNob)
{
	assert(pNob->IsAnyNeuron());
	return static_cast<Neuron *>(pNob);
}

void Neuron::AppendMenuItems(AddMenuFunc const & add) const
{
	if (IsNeuron())
		add(IDD_SPLIT_NEURON);        
	BaseKnot::AppendMenuItems(add);
}
