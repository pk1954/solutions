// Synapse.cpp 
//
// NNetModel

module;

#include <cmath>
#include <cassert>
#include <iostream>
#include "Resource.h"

module NNetModel:Synapse;

import SimulationTime;
import DrawContext;
import Types;
import Util;
import :NNetColors;
import :NNetParameters;
import :PosNob;

using std::wcout;
using std::endl;
using std::fabs;

Synapse::Synapse(MicroMeterPnt const& center)
  : m_pos(center)
{
	RecalcDelayBuffer();
}

void Synapse::RecalcDelayBuffer()
{
	fMicroSecs umDelay { GetParam()->SynapseDelay() };
	fMicroSecs umRes   { GetParam()->TimeResolution() };
	size_t     bufSize { static_cast<size_t>(umDelay / umRes) };
	m_pulseBuffer.Resize(bufSize, 0.0_mV);
}

void Synapse::ClearDynamicData()
{
	PosNob::ClearDynamicData();
    m_usBlockStartTime = fMicroSecs::NULL_VAL();
    m_mVaddInput       = 0._mV;
    m_pulseBuffer.Fill(0._mV);
}

void Synapse::AddIncoming(Pipe* pPipe)
{
	m_pPipeIn = pPipe;
	m_bDirty = true;
}

void Synapse::AddOutgoing(Pipe* pPipe)
{
	m_pPipeOut = pPipe;
	m_bDirty = true;
}

void Synapse::SetAddPipe(Pipe* const pPipe)
{
	m_pPipeAdd = pPipe;
	m_bDirty = true;
}

void Synapse::SetPosNoFix(MicroMeterPnt const& newPos) 
{ 
	m_pos = newPos; 
	m_bDirty = true;
}

void Synapse::RotateNob(MicroMeterPnt const& umPntPivot, Radian const radDelta)
{
	m_pos.Rotate(umPntPivot, radDelta);
	m_bDirty = true;
}

void Synapse::SetAllIncoming(PosNob& src)
{
	assert(src.IsSynapse());
	Synapse* pSynapseSrc { static_cast<Synapse*>(&src) };
	SetAddPipe(pSynapseSrc ->m_pPipeAdd);
	AddIncoming(pSynapseSrc->m_pPipeIn);
}

void Synapse::SetAllOutgoing(PosNob& src)
{
	assert(src.IsSynapse());
	Synapse* pSynapseSrc { static_cast<Synapse*>(&src) };
	AddOutgoing(pSynapseSrc->m_pPipeOut);
}

void Synapse::MoveNob(MicroMeterPnt const& delta)
{
	SetPos(GetPos() + delta);
	m_pPipeAdd->PosChanged();
	m_pPipeIn ->PosChanged();
	m_pPipeOut->PosChanged();
	m_bDirty = true;
}

void Synapse::calcPosition() const
{
	if (m_bDirty)
	{
		static float      const SQRT3       { sqrtf(3.0f) };
		static float      const SQRT3DIV3   { SQRT3 / 3.0f };
		static MicroMeter const GAP         { PIPE_WIDTH * 0.1f };
		static MicroMeter const CENTER_DIST { EXTENSION * SQRT3DIV3 };
		static MicroMeter const OFF_DIST    { PIPE_HALF + GAP + RADIUS }; // distance from mainpos to base line

		MicroMeterPnt const umPntBaseVector { m_pPipeOut->GetEndPoint() - m_pPipeIn->GetStartPoint() };
		MicroMeterPnt const umPntOrtho      { umPntBaseVector.OrthoVector() };
		MicroMeterPnt const umPntAddDir     { GetPos() - m_pPipeAdd->GetStartPoint() };
		MicroMeter    const umCrit          { CrossProduct(umPntBaseVector, umPntAddDir) };
		float         const fDirection      { (umCrit > 0._MicroMeter) ? 1.0f : -1.0f };
		MicroMeterPnt const umPntCenter     { GetPos() + umPntOrtho.ScaledTo((CENTER_DIST + OFF_DIST) * fDirection) };
		MicroMeter    const umTop           { (CENTER_DIST * 2.0f) * fDirection };
		MicroMeter    const umBase          { - CENTER_DIST * fDirection };
		MicroMeterPnt const umPntBase       { umPntCenter + umPntOrtho.ScaledTo(umBase) };
		MicroMeterPnt const umPntVecScaled  { umPntBaseVector.ScaledTo(EXTENSION) };
		m_umPntBase1  = umPntBase + umPntVecScaled;
		m_umPntBase2  = umPntBase - umPntVecScaled;
		m_umPntTop    = umPntCenter + umPntOrtho.ScaledTo(umTop);
		m_umPntCenter = umPntCenter;
		m_bDirty = false;
	}
}

MicroMeterPnt Synapse::GetAddPos() const       
{ 
	calcPosition();
    return m_umPntTop;
}

void Synapse::Recalc()
{
	calcPosition();
	m_usBlock.clear();
}

void Synapse::Check() const
{
	PosNob::Check();
	assert(m_pPipeAdd);
	assert(m_pPipeIn);
	assert(m_pPipeOut);
	assert(m_pPipeAdd->GetEndKnotId  () == GetId());
	assert(m_pPipeIn ->GetEndKnotId  () == GetId());
	assert(m_pPipeOut->GetStartKnotId() == GetId());
	m_pPipeAdd->Check();
	m_pPipeIn ->Check();
	m_pPipeOut->Check();
}

void Synapse::Dump() const
{
	Nob::Dump();
	wcout << L" In";
	m_pPipeIn->Dump();
	wcout << L" Out";
	m_pPipeOut->Dump();
	wcout << L" Add";
	m_pPipeAdd->Dump();
	//if (m_bOutputBlocked)
	//	wcout << L"blocked since " << m_usBlocked << L"µs" << endl;
}

void Synapse::PosChanged()
{
	m_bDirty = true;
}

void Synapse::Reconnect()
{
	m_pPipeAdd->SetEndPnt(this);
	m_pPipeAdd->PosChanged();
}

void Synapse::Link(Nob const& nobSrc, Nob2NobFunc const& f)
{
	Synapse const& src { static_cast<Synapse const&>(nobSrc) };
	m_pPipeIn  = static_cast<Pipe*>(f(src.m_pPipeIn));
	m_pPipeOut = static_cast<Pipe*>(f(src.m_pPipeOut));
	m_pPipeAdd = static_cast<Pipe*>(f(src.m_pPipeAdd));
	m_bDirty = true;
}

bool Synapse::Includes(MicroMeterPnt const& point) const
{
	MicroMeterCircle const circle               { m_pos, KNOT_WIDTH };
	bool             const bCircleIncludesPnt   { circle.Includes(point) };

	MicroMeterPnt const umPntBaseVector { m_pPipeOut->GetEndPoint() - m_pPipeIn->GetStartPoint() };
	MicroMeterPnt const umPntOrtho      { umPntBaseVector.OrthoVector() };
	MicroMeterPnt const umPntAddDir     { GetPos() - m_pPipeAdd->GetStartPoint() };
	MicroMeter    const umCrit          { CrossProduct(umPntBaseVector, umPntAddDir) };
	float         const fDirection      { (umCrit > 0._MicroMeter) ? 1.0f : -1.0f };
	MicroMeterPnt const umPntCenter     { GetPos() + umPntOrtho.ScaledTo((CENTER_DIST + OFF_DIST) * fDirection) };

	bool             const bTriangleIncludesPnt { Distance(point, umPntCenter) <= EXTENSION	};
	return bCircleIncludesPnt || bTriangleIncludesPnt;
}

void Synapse::ReplaceIncoming(Pipe* const pDel, Pipe* const pNew)
{
	if (pDel == m_pPipeAdd)
		m_pPipeAdd = pNew;
	else if (pDel == m_pPipeIn)
		m_pPipeIn = pNew;
	else
		assert(false);
	m_bDirty = true;
}

void Synapse::ReplaceOutgoing(Pipe* const pDel, Pipe* const pNew)
{
	assert(pDel == m_pPipeOut);
	m_pPipeOut = pNew;
	m_bDirty = true;
}

void Synapse::Apply2AllInPipes(PipeFunc const& f)
{
	f(*m_pPipeIn);
	f(*m_pPipeAdd);
}

void Synapse::Apply2AllOutPipes(PipeFunc const& f)
{
	f(*m_pPipeOut);
}

void Synapse::Apply2AllInPipesC(PipeFuncC const& f) const
{
	f(*m_pPipeIn);
	f(*m_pPipeAdd);
}

void Synapse::Apply2AllOutPipesC(PipeFuncC const& f) const
{
	f(*m_pPipeOut);
}

bool Synapse::Apply2AllInPipesB(PipeCrit const& c) const
{
	return c(*m_pPipeIn) || c(*m_pPipeAdd);
}

bool Synapse::Apply2AllOutPipesB(PipeCrit const& c) const
{
	return c(*m_pPipeOut);
}

void Synapse::drawSynapse
(
	DrawContext const& context, 
	MicroMeter  const  umRadius,
	Color       const  col
) const
{
	calcPosition();
	context.FillCircle(MicroMeterCircle(m_umPntTop,   umRadius), col);
	context.FillCircle(MicroMeterCircle(m_umPntBase1, umRadius), col);
	context.FillCircle(MicroMeterCircle(m_umPntBase2, umRadius), col);
	context.DrawLine(MicroMeterLine    (m_umPntTop,   m_umPntBase1), umRadius * 2.0f, col);
	context.DrawLine(MicroMeterLine    (m_umPntTop,   m_umPntBase2), umRadius * 2.0f, col);
	context.DrawLine(MicroMeterLine    (m_umPntBase1, m_umPntBase2), umRadius * 2.0f, col);
	//if (!m_bOutputBlocked)
	//	context.DrawLine(MicroMeterLine(umPntBase1, umPntBase2), umRadius * 2.0f, D2D1::ColorF::Yellow);

	//MicroMeterRect const rect
	//(
	//	umPntBase.GetX() - umRadius,
	//	umPntBase.GetY() - umRadius * 2.0f,
	//	umPntBase.GetX() + umRadius,
	//	umPntBase.GetY() + umRadius * 2.0f
	//);
	//Degrees angle { Vector2Degrees(umPntOrtho) };

	//context.SetRotation(angle, m_umPntCenter);
	//context.FillRoundedRectangle(rect, col, umRadius);
	//angle += 120.0_Degrees;

	//context.SetRotation(angle, m_umPntCenter);
	//context.FillRoundedRectangle(rect, col, umRadius);
	//angle += 120.0_Degrees;

	//context.SetRotation(angle, m_umPntCenter);
	//context.FillRoundedRectangle(rect, col, umRadius);

	//context.Reset();
}

void Synapse::DrawExterior(DrawContext const& context, tHighlight const type) const
{
	MicroMeterCircle const circle { m_pos, KNOT_WIDTH };
	Color            const col    { GetExteriorColor(type) };
	drawSynapse(context, RADIUS, col);
	context.FillCircle(circle, col);
}

void Synapse::DrawInterior(DrawContext const& context, tHighlight const type) const
{ 
	Color const col { GetInteriorColor(type, m_mVaddInput) };
	drawSynapse(context, RADIUS * PIPE_INTERIOR, col);
	if (m_bBlockActive)
		FillInternalCircle(context, tHighlight::blocked);
	else
		FillInternalCircle(context, type);
}

void Synapse::CollectInput()
{
	m_mVpotential = m_pPipeIn ->GetPotential();
	m_mVaddInput  = m_pPipeAdd->GetPotential();
	m_pulseBuffer.Push(m_mVaddInput);
}

bool Synapse::CompStep()
{
	static const mV PULSE_THRESHOLD { 0._mV };
	
	mV mVfromBuffer = m_pulseBuffer.Get();
	m_mVpotential += mVfromBuffer;

	bool bSignal      { (m_mVpotential > PULSE_THRESHOLD) };
	bool bInPulseDist { SimulationTime::Get() - m_usBlockStartTime < GetParam()->PulseDistMin() };

	switch (m_state)
	{
		using enum tState;
	case idle:
		if (bSignal)
			startLeadPulse();
		break;

	case leadPulse:
		if (!bInPulseDist)
		{
			m_state = idle;  // leading pulse longer than min pulse dist!
		}
		else if (!bSignal)
		{
			block();
			m_state = blockedIdle;
		}
		break;

	case blockedIdle:
		if (bSignal)
		{
			m_mVpotential.Set2Zero();  // block this pulse
			if (bInPulseDist)
				m_state = blockedPulse;
			else
				startLeadPulse();
		}
		else
		{
			if (!bInPulseDist)    // block time over and no signal: unblock
				unblock();
		}
		break;

	case blockedPulse:
		if (bSignal)
		{
			m_mVpotential.Set2Zero();  // block this pulse
			if (!bInPulseDist)
				m_state = trailPulse; 
		}
		else   // blocked pulse ended 
		{
			if (bInPulseDist)
				m_state = blockedIdle;  
			else            
				unblock();   // block time over and no signal: unblock
		}
		break;

	case trailPulse:
		if (bSignal)
			m_mVpotential.Set2Zero();  // block this pulse
		else 
			unblock();
		break;
	}

	return false;  // no stop on trigger
}

void Synapse::unblock()
{
	m_usBlock.push_back(SimulationTime::Get());
	m_bBlockActive = false;
	m_state = tState::idle;
}

void Synapse::block()
{
	m_bBlockActive = true;
	m_usBlock.push_back(SimulationTime::Get()); // block output
}

void Synapse::startLeadPulse()
{
	m_usBlockStartTime = SimulationTime::Get();
	m_state = tState::leadPulse;
}

void Synapse::AppendMenuItems(AddMenuFunc const& add) const
{
	Nob::AppendMenuItems(add);
	add(IDD_DETACH_NOB);
}

void Synapse::SelectAllConnected(bool const bFirst)
{
	if (!IsSelected() || bFirst)
	{
		Nob::Select(true);
		m_pPipeIn ->SelectAllConnected(false);
		m_pPipeOut->SelectAllConnected(false);
		m_pPipeAdd->SelectAllConnected(false);
	}
}
