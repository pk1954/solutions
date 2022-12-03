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
import Util;
import :NNetColors;
import :NNetParameters;
import :PosNob;

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
	resetPos(pPipeAdd->GetEndPoint());
}

MicroMeterPnt Synapse::GetPos() const
{
	return m_umPntPipeAnchor;
}

void Synapse::RotateNob(MicroMeterPnt const& umPntPivot, Radian const radDelta)
{  // TODO
//	(position) .Rotate(umPntPivot, radDelta);
}

void Synapse::SetAllIncoming(PosNob& src)
{
	assert(src.IsSynapse());
	Synapse* pSynapseSrc { static_cast<Synapse*>(&src) };
	m_pPipeAdd  = pSynapseSrc->m_pPipeAdd;
	m_pPipeMain = pSynapseSrc->m_pPipeMain;
}

void Synapse::SetAllOutgoing(PosNob& src)
{
	assert(src.IsSynapse());
	Synapse* pSynapseSrc { static_cast<Synapse*>(&src) };
	m_pPipeMain = pSynapseSrc->m_pPipeMain;
}

void Synapse::SetPos(MicroMeterPnt const& newPos)
{
	MoveNob(newPos - GetPos());
}

void Synapse::MoveNob(MicroMeterPnt const& delta)
{
	MicroMeterPnt const umPntNew    { GetPos() + delta };
	MicroMeter    const umDist      { m_pPipeMain->DistPntToPipe(umPntNew) };
	MicroMeterPnt const umPntOrtho  { m_pPipeMain->GetVector().OrthoVector() };
	MicroMeterPnt const umPntScaled { umPntOrtho.ScaledTo(umDist) };
	MicroMeterPnt const umPntRoot   { umPntNew + umPntScaled };
	resetPos(umPntRoot);
	m_pPipeAdd->PositionChanged();
}

void Synapse::RemoveFromMainPipe() 
{ 
	m_pPipeMain->RemoveSynapse(this); 
}

void Synapse::Add2MainPipe() 
{ 
	m_pPipeMain->AddSynapse(this); 
}

void Synapse::SetMainPipe(Pipe* const pPipe)
{
	MicroMeterPnt umPntPos { GetPos() };
	m_pPipeMain = pPipe;
	resetPos(umPntPos);
}

void Synapse::ChangeMainPipe(Pipe* const pPipeNew)
{
	m_pPipeMain->RemoveSynapse(this);
	pPipeNew->AddSynapse(this);
	SetMainPipe(pPipeNew);
}

void Synapse::resetPos(MicroMeterPnt const& newPos)
{
	m_fPosOnMainPipe = m_pPipeMain->PosOnPipe(newPos);
	m_fPosOnMainPipe = ClipToMinMax(m_fPosOnMainPipe, 0.0f, 1.0f);
	RecalcPos();
}

void Synapse::RecalcPos() const
{
	static const MicroMeter GAP         { PIPE_WIDTH * 0.1f };
	static const MicroMeter CENTER_DIST { EXTENSION * SQRT3DIV3 };
	static const MicroMeter TOP_DIST    { EXTENSION * SQRT3 };
	static const MicroMeter OFF_DIST    { PIPE_HALF + GAP + RADIUS }; // distance from mainpos to base line

	MicroMeterPnt const umPntVector  { m_pPipeMain->GetVector() };
	MicroMeterPnt const umPntAddDir  { m_pPipeAdd->GetStartPoint() - m_pPipeMain->GetStartPoint() };
	MicroMeterPnt const umPntMainPos { m_pPipeMain->GetStartPoint() + m_pPipeMain->GetVector() * m_fPosOnMainPipe };
	MicroMeterPnt const umPntOrtho   { umPntVector.OrthoVector() };
	MicroMeter    const umCrit       { CrossProduct(umPntVector, umPntAddDir) };

	m_fDirection = (umCrit < 0._MicroMeter) ? 1.0f : -1.0f;
	m_umPntPipeAnchor = umPntMainPos + umPntOrtho.ScaledTo((TOP_DIST    + OFF_DIST) * m_fDirection);
	m_umPntCenter     = umPntMainPos + umPntOrtho.ScaledTo((CENTER_DIST + OFF_DIST) * m_fDirection);
}

void Synapse::Check() const
{
	Nob::Check();
	assert(m_pPipeAdd);
	assert(m_pPipeMain);
	assert(m_pPipeAdd->GetEndKnotId() == GetId());
	assert(m_pPipeMain->IsConnectedSynapse(*this));
	m_pPipeAdd->Check();
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

bool Synapse::Includes(MicroMeterPnt const& point) const
{
	return Distance(point, GetPos()) <= GetExtension();
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

void Synapse::drawSynapse
(
	DrawContext  const& context, 
	MicroMeter   const  umRadius,
	MicroMeter   const  umSize,
	D2D1::ColorF const  col
) const
{
	MicroMeter    const umTop          { umSize * ( 2.0f * SQRT3DIV3) * m_fDirection };
	MicroMeter    const umBase         { umSize * (-1.0f * SQRT3DIV3) * m_fDirection };
	MicroMeterPnt const umPntOrtho     { m_pPipeMain->GetVector().OrthoVector() };
	MicroMeterPnt const umPntTop       { m_umPntCenter + umPntOrtho.ScaledTo(umTop) };
	MicroMeterPnt const umPntBase      { m_umPntCenter + umPntOrtho.ScaledTo(umBase) };
	MicroMeterPnt const umPntVecScaled { m_pPipeMain->GetVector().ScaledTo(umSize) };
	MicroMeterPnt const umPntBase1     { umPntBase + umPntVecScaled };
	MicroMeterPnt const umPntBase2     { umPntBase - umPntVecScaled };

	context.FillCircle(MicroMeterCircle(umPntTop,   umRadius), col);
	context.FillCircle(MicroMeterCircle(umPntBase1, umRadius), col);
	context.FillCircle(MicroMeterCircle(umPntBase2, umRadius), col);
	context.DrawLine(MicroMeterLine(umPntTop,   umPntBase1), umRadius * 2.0f, col);
	context.DrawLine(MicroMeterLine(umPntTop,   umPntBase2), umRadius * 2.0f, col);
	context.DrawLine(MicroMeterLine(umPntBase1, umPntBase2), umRadius * 2.0f, col);
}

void Synapse::DrawExterior(DrawContext const& context, tHighlight const type) const
{
	drawSynapse(context, RADIUS, EXTENSION, GetExteriorColor(type));
}

void Synapse::DrawInterior(DrawContext const& context, tHighlight const type) const
{ 
	static float const INTERIOR { 0.75f };
	drawSynapse(context, RADIUS * INTERIOR, EXTENSION - RADIUS * (1.0f - INTERIOR), GetInteriorColor(type));
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

Synapse const* Cast2Synapse(Nob const* pNob)
{
	assert(pNob);
	assert(pNob->IsSynapse());
	return static_cast<Synapse const*>(pNob);
}

Synapse* Cast2Synapse(Nob* pNob)
{
	assert(pNob);
	assert(pNob->IsSynapse());
	return static_cast<Synapse*>(pNob);
}
