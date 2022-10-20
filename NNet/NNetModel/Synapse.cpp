// Synapse.cpp 
//
// NNetModel

module;

#include <cassert>
#include <iostream>
#include "Resource.h"

module NNetModel:Synapse;

import DrawContext;
import Types;
import :NNetColors;
import :NNetParameters;

void Synapse::Check() const
{
	Knot::Check();
}

//void Synapse::Dump() const
//{
//	Nob::Dump();
//	wcout << L" in";
//	m_pPipeIn->Dump();
//	wcout << L" out";
//	m_pPipeOut->Dump();
//	wcout << L" add";
//	m_pPipeOut->Dump();
//	wcout << endl;
//}

void Synapse::DrawExterior(DrawContext const& context, tHighlight const type) const
{
	Knot::DrawExterior(context, type);
	MicroMeterPnt    const umPos(GetAddPipe().GetEndPoint());
	MicroMeter       const umSize(GetExtension() * 1.5f);
	MicroMeterCircle const umCircle(umPos, umSize);
	FillExternalCircle(context, umCircle, type);
}

void Synapse::DrawInterior(DrawContext const& context, tHighlight const type) const
{
	Knot::DrawInterior(context, type);
	MicroMeterPnt    const umPos   (GetAddPipe().GetEndPoint());
	MicroMeter       const umSize  (GetExtension() * 1.5f);
	MicroMeterCircle const umCircle(umPos, umSize);
	FillInternalCircle(context, umCircle, type);
}

void Synapse::CollectInput()
{
	m_mVinputBuffer = static_cast<Pipe const&>(GetIncoming(0)).GetEndKnotPtr()->GetNextOutput();
	m_mVaddInput    = static_cast<Pipe const&>(GetIncoming(1)).GetEndKnotPtr()->GetNextOutput();
}

bool Synapse::CompStep()
{
	if (m_state == tState::normal)
	{
	    mV const mVthreshold { GetParam()->SynapseThreshold() };
		if (m_mVinputBuffer >= mVthreshold)
		{
			m_state = tState::stdInputBlocked;
			m_usBlocked = 0.0_MicroSecs;
		}
		else if (m_mVaddInput >= mVthreshold)
		{
			m_state = tState::addLineBlocked;
			m_usBlocked = 0.0_MicroSecs;
		}
	}
	else  // one input blocked
	{
		m_usBlocked += GetParam()->TimeResolution();
		if (m_usBlocked >= GetParam()->SynapseBlockTime())
			m_state = tState::normal;
	}
	return false;  // no stop on trigger
}

mV Synapse::GetNextOutput() const
{
	switch (m_state)
	{
		case tState::normal:          return m_mVinputBuffer + m_mVaddInput;
		case tState::addLineBlocked:  return m_mVinputBuffer;
		case tState::stdInputBlocked: return m_mVaddInput;
		default: assert(false);
	}
	return 0._mV;
}
