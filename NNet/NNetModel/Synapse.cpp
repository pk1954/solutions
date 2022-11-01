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
import :BaseKnot;

using std::wcout;
using std::endl;

void Synapse::Check() const
{
	Nob::Check();
	assert(GetNrOfInConns () == 2);
	assert(GetNrOfOutConns() == 0);
	GetAddPipe ().Check();
	GetMainPipe().Check();
	assert(GetAddPipe().GetEndKnotId() == GetId());
	assert(GetMainPipe().IsConnectedSynapse(*this));
}

void Synapse::Dump() const
{
	BaseKnot::Dump();
	wcout << L" tState " << static_cast<int>(m_state) << endl;
	wcout << L" blocked " << m_usBlocked << endl;
	wcout << L" addInput " << m_mVaddInput << endl;
}

void Synapse::Reconnect()
{
	getMainPipe().AddSynapse(this);
	getAddPipe ().SetEndPnt(this);
}

void Synapse::DrawExterior(DrawContext const& context, tHighlight const type) const
{
	//MicroMeter const umRadius { PIPE_WIDTH };
	//Ortho
	//MicroMeterPnt    const umPos(GetAddPipe().GetEndPoint());
	//MicroMeter       const umSize(GetExtension() * 1.5f);
	//MicroMeterCircle const umCircle(umPos, umSize);
	//FillExternalCircle(context, umCircle, type);
}

void Synapse::DrawInterior(DrawContext const& context, tHighlight const type) const
{
	//BaseKnot::DrawInterior(context, type);
	//MicroMeterPnt    const umPos   (GetAddPipe().GetEndPoint());
	//MicroMeter       const umSize  (GetExtension() * 1.5f);
	//MicroMeterCircle const umCircle(umPos, umSize);
	//FillInternalCircle(context, umCircle, type);
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
