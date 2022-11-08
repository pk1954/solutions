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
	Pipe * const pPipeMain,
	Pipe * const pPipeAdd
)
  : PosNob(NobType::Value::synapse),
	m_pPipeMain(pPipeMain),
	m_pPipeAdd(pPipeAdd)
{
	ResetPos(pPipeAdd->GetEndPoint());
}

MicroMeterPnt Synapse::GetPos() const
{
	return m_pPipeMain->GetStartPoint() + m_pPipeMain->GetVector() * m_fPosOnMainPipe;
}

void Synapse::RotateNob(MicroMeterPnt const& umPntPivot, Radian const radDelta)
{  // TODO
//	(position) .Rotate(umPntPivot, radDelta);
}

void Synapse::SetPos(MicroMeterPnt const& newPos)
{
	MoveNob(newPos - GetPos());
}

void Synapse::MoveNob(MicroMeterPnt const& delta)
{
	m_pPipeMain->MoveNob(delta);
}

void Synapse::Check() const
{
	Nob::Check();
	m_pPipeAdd->Check();
	m_pPipeMain->Check();
	assert(m_pPipeAdd->GetEndKnotId() == GetId());
	assert(m_pPipeMain->IsConnectedSynapse(*this));
}

void Synapse::Dump() const
{
	Nob::Dump();
	wcout << L" main";
	m_pPipeMain->Dump();
	wcout << L" Add";
	m_pPipeAdd->Dump();
	wcout << L" tState " << static_cast<int>(m_state) << endl;
	wcout << L" blocked " << m_usBlocked << endl;
	wcout << L" addInput " << m_mVaddInput << endl;
}

void Synapse::SetMainPipe(Pipe* const pPipe)
{
	MicroMeterPnt umPntPos { GetPos() };
	m_pPipeMain = pPipe;
	ResetPos(umPntPos);
}

void Synapse::Reconnect()
{
	m_pPipeAdd->SetEndPnt(this);
}

void Synapse::Link(Nob const& nobSrc, Nob2NobFunc const& f)
{
	Synapse const& src { static_cast<Synapse const&>(nobSrc) };
	m_pPipeMain = static_cast<Pipe*>(f(src.m_pPipeMain));
	m_pPipeAdd  = static_cast<Pipe*>(f(src.m_pPipeAdd));
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

void Synapse::ResetPos(MicroMeterPnt const& newPos)
{
	m_fPosOnMainPipe = m_pPipeMain->PosOnPipe(newPos);
}

void Synapse::ReplaceIncoming(Pipe* const pDel, Pipe* const pAdd)
{
	assert(pDel == m_pPipeAdd);
	m_pPipeAdd = pAdd;
}

void Synapse::ReplaceOutgoing(Pipe* const pDel, Pipe* const pAdd)
{
	assert(false);
}

void Synapse::Apply2AllInPipes(PipeFunc const& f) const
{
	f(*m_pPipeMain);
	f(*m_pPipeAdd);
}

void Synapse::Apply2AllOutPipes(PipeFunc const& f) const
{
	f(*m_pPipeMain);
}

bool Synapse::Apply2AllInPipesB(PipeCrit const& c) const
{
	return c(*m_pPipeMain) || c(*m_pPipeAdd);
}

bool Synapse::Apply2AllOutPipesB(PipeCrit const& c) const
{
	return c(*m_pPipeMain);
}

void Synapse::DrawExterior(DrawContext const& context, tHighlight const type) const
{ // TODO
	//MicroMeter const umRadius { PIPE_WIDTH };
	//Ortho
	//MicroMeterPnt    const umPos(GetAddPipe().GetEndPoint());
	//MicroMeter       const umSize(GetExtension() * 1.5f);
	//MicroMeterCircle const umCircle(umPos, umSize);
	//FillExternalCircle(context, umCircle, type);
}

void Synapse::DrawInterior(DrawContext const& context, tHighlight const type) const
{ // TODO
	//BaseKnot::DrawInterior(context, type);
	//MicroMeterPnt    const umPos   (GetAddPipe().GetEndPoint());
	//MicroMeter       const umSize  (GetExtension() * 1.5f);
	//MicroMeterCircle const umCircle(umPos, umSize);
	//FillInternalCircle(context, umCircle, type);
}

void Synapse::CollectInput()
{
	m_mVinputBuffer = m_pPipeMain->GetEndNobPtr()->GetNextOutput();
	m_mVaddInput    = m_pPipeAdd ->GetEndNobPtr()->GetNextOutput();
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
