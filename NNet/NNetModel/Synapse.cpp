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

Synapse::Synapse(MicroMeterPnt const& center)
	: PosNob(NobType::Value::synapse),
	m_circle(center, KNOT_WIDTH)
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

Synapse::Synapse(Synapse const & rhs)
	: PosNob(NobType::Value::synapse)
{
	PosNob::operator=(rhs);
	m_pPipeIn  = rhs.m_pPipeIn;
	m_pPipeOut = rhs.m_pPipeOut;
	m_pPipeAdd = rhs.m_pPipeAdd;
}

void Synapse::AddIncoming(Pipe* pPipe)
{
	m_pPipeIn = pPipe;
}

void Synapse::AddOutgoing(Pipe* pPipe)
{
	m_pPipeOut = pPipe;
}

void Synapse::SetAddPipe(Pipe* const pPipe)
{
	m_pPipeAdd = pPipe;
}

void Synapse::SetPosNoFix(MicroMeterPnt const& newPos) 
{ 
	m_circle.SetPos(newPos); 
}

void Synapse::RotateNob(MicroMeterPnt const& umPntPivot, Radian const radDelta)
{
	m_circle.Rotate(umPntPivot, radDelta);
	recalcPosition();
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
	recalcPosition();
}

void Synapse::recalcPosition() const
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
	MicroMeter    const umTop           { EXTENSION * ( 2.0f * SQRT3DIV3) * fDirection };
	MicroMeter    const umBase          { EXTENSION * (-1.0f * SQRT3DIV3) * fDirection };
	MicroMeterPnt const umPntBase       { umPntCenter + umPntOrtho.ScaledTo(umBase) };
	MicroMeterPnt const umPntVecScaled  { umPntBaseVector.ScaledTo(EXTENSION) };
	m_umPntBase1  = umPntBase + umPntVecScaled;
	m_umPntBase2  = umPntBase - umPntVecScaled;
	m_umPntTop    = umPntCenter + umPntOrtho.ScaledTo(umTop);
	m_umPntCenter = umPntCenter;
}

void Synapse::Recalc()
{
	recalcPosition();
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
	if (m_bOutputBlocked)
		wcout << L"blocked since " << m_usBlocked.GetValue() << L"µs" << endl;
}

void Synapse::PosChanged()
{
	recalcPosition();
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
}

bool Synapse::Includes(MicroMeterPnt const& point) const
{
	bool bCircleIncludesPnt   { m_circle.Includes(point) };
	bool bTriangleIncludesPnt { Distance(point, m_umPntCenter) <= EXTENSION	};
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
}

void Synapse::ReplaceOutgoing(Pipe* const pDel, Pipe* const pNew)
{
	assert(pDel == m_pPipeOut);
	m_pPipeOut = pNew;
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
	DrawContext  const& context, 
	MicroMeter   const  umRadius,
	D2D1::ColorF const  col
) const
{
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
	ColorF col { GetExteriorColor(type) };
	drawSynapse(context, RADIUS, col);
	context.FillCircle(m_circle, col);
}

void Synapse::DrawInterior(DrawContext const& context, tHighlight const type) const
{ 
	D2D1::ColorF const col { GetInteriorColor(type, m_mVaddInput) };
	drawSynapse(context, RADIUS * PIPE_INTERIOR, col);
	if (m_bOutputBlocked)
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
	fMicroSecs usBlockTime = GetParam()->PulseDistMin();
	if (m_bOutputBlocked)
	{
		m_mVpotential = 0.0_mV;
		m_usBlocked += GetParam()->TimeResolution();
		if (m_usBlocked > usBlockTime)
			m_bOutputBlocked = false;
	}
	else
	{
		m_mVpotential += m_pulseBuffer.Get();
		if (m_mVpotential >= GetParam()->SynapseThreshold())
		{
			m_bOutputBlocked = true;
			m_usBlocked = 0.0_MicroSecs;
		}
	}
	return false;  // no stop on trigger
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
