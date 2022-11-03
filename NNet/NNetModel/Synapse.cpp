// Synapse.cpp 
//
// NNetModel

module;

#include <cmath>
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
using std::fabs;

Synapse::Synapse
(
	Pipe& pPipeMain,
	Pipe& pPipeAdd
)
  : Nob(NobType::Value::synapse),
	m_pipeMain(pPipeMain),
	m_pipeAdd(pPipeAdd)
{
	MicroMeterPnt umPntMainVector { pPipeMain.GetVector() };
	if (umPntMainVector.GetX().GetAbsValue() > umPntMainVector.GetY().GetAbsValue())
	{
		m_fPosOnMainPipe = (m_pipeAdd.GetEndPoint().GetX() - pPipeMain.GetStartPoint().GetX()) / umPntMainVector.GetX();
	}
	else
	{
		m_fPosOnMainPipe = (m_pipeAdd.GetEndPoint().GetY() - pPipeMain.GetStartPoint().GetY()) / umPntMainVector.GetY();
	}
}

MicroMeterPnt Synapse::GetPos() const
{
	return m_pipeMain.GetStartPoint() + m_pipeMain.GetVector() * m_fPosOnMainPipe;
}

void Synapse::RotateNob(MicroMeterPnt const& umPntPivot, Radian const radDelta)
{
//	(position) .Rotate(umPntPivot, radDelta);
}

void Synapse::SetPos(MicroMeterPnt const& newPos)
{
	MoveNob(newPos - GetPos());
}

void Synapse::MoveNob(MicroMeterPnt const& delta)
{
	m_pipeMain.MoveNob(delta);
}

void Synapse::Check() const
{
	Nob::Check();
	m_pipeAdd.Check();
	m_pipeMain.Check();
	assert(m_pipeAdd.GetEndKnotId() == GetId());
	assert(m_pipeMain.IsConnectedSynapse(*this));
}

void Synapse::Dump() const
{
	Nob::Dump();
	wcout << L" main";
	m_pipeMain.Dump();
	wcout << L" Add";
	m_pipeAdd.Dump();
	wcout << L" tState " << static_cast<int>(m_state) << endl;
	wcout << L" blocked " << m_usBlocked << endl;
	wcout << L" addInput " << m_mVaddInput << endl;
}

void Synapse::Reconnect()
{
	m_pipeMain.AddSynapse(this);
	m_pipeAdd.SetEndPnt(this);
}

void Synapse::Link(Nob const& nobSrc, Nob2NobFunc const& f)
{
	Synapse const& src { static_cast<Synapse const&>(nobSrc) };
	m_pipeMain = static_cast<Pipe&>(*f(&src.m_pipeMain));
	m_pipeAdd  = static_cast<Pipe&>(*f(&src.m_pipeAdd));
}

bool Synapse::IsIncludedIn(MicroMeterRect const& umRect) const
{
	return umRect.Includes(GetPos());
}

void Synapse::Expand(MicroMeterRect& umRect) const
{
	umRect.Expand(GetPos());
}

bool Synapse::Includes(MicroMeterPnt const& point) const
{
	return Distance(point, GetPos()) <= GetExtension();
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
	m_mVinputBuffer = m_pipeMain.GetEndKnotPtr()->GetNextOutput();
	m_mVaddInput    = m_pipeAdd .GetEndKnotPtr()->GetNextOutput();
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
