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
	Pipe* const pPipeMain,
	Pipe* const pPipeAdd
)
	: PosNob(NobType::Value::synapse),
	m_pPipeMain(pPipeMain),
	m_pPipeAdd(pPipeAdd)
{}

Synapse::Synapse
(
	Nob* const pNobPipeMain,
	Nob* const pNobPipeAdd
)
	: PosNob(NobType::Value::synapse),
	m_pPipeMain(static_cast<Pipe*>(pNobPipeMain)),
	m_pPipeAdd(static_cast<Pipe*>(pNobPipeAdd))
{}

Synapse::Synapse(Synapse const & rhs)
	: PosNob(NobType::Value::synapse)
{
	PosNob::operator=(rhs);
	m_pPipeMain      = rhs.m_pPipeMain;
	m_pPipeAdd       = rhs.m_pPipeAdd;
	m_fPosOnMainPipe = rhs.m_fPosOnMainPipe;
	RecalcPositions();
}

void Synapse::SetPosOnMainPipe(float const fPosNew)
{
	assert(fPosNew >= 0.0f);
	assert(fPosNew <= 1.0f);
	m_fPosOnMainPipe = fPosNew;
}

MicroMeterPnt Synapse::GetPos() const
{
	return m_umPntPipeAnchor;
}

void Synapse::RotateNob(MicroMeterPnt const& umPntPivot, Radian const radDelta)
{
	RecalcPositions();
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

void Synapse::MoveNob(MicroMeterPnt const& delta)
{
	MicroMeterPnt const umPntNew    { GetPos() + delta };
	MicroMeter    const umDist      { m_pPipeMain->DistPntToPipe(umPntNew) };
	MicroMeterPnt const umPntOrtho  { m_pPipeMain->GetVector().OrthoVector() };
	MicroMeterPnt const umPntScaled { umPntOrtho.ScaledTo(umDist) };
	MicroMeterPnt const umPntRoot   { umPntNew + umPntScaled };
	RecalcAll(umPntRoot);
	m_pPipeAdd->PosChanged();
}

void Synapse::RemoveFromMainPipe() 
{ 
	m_pPipeMain->RemoveSynapse(this); 
}

void Synapse::Add2MainPipe() 
{ 
	m_pPipeMain->AddSynapse(this); 
}

void Synapse::SetAddPipe(Pipe* const pPipe)
{
	m_pPipeAdd = pPipe;
}

void Synapse::SetMainPipe(Pipe* const pPipe)
{
	MicroMeterPnt umPntPos { GetPos() };
	m_pPipeMain = pPipe;
	RecalcAll(umPntPos);
}

void Synapse::ChangeMainPipe(Pipe* const pPipeNew)
{
	m_pPipeMain->RemoveSynapse(this);
	pPipeNew->AddSynapse(this);
	SetMainPipe(pPipeNew);
}

void Synapse::RecalcAll(MicroMeterPnt const& newPos)
{
	m_fPosOnMainPipe = m_pPipeMain->PosOnPipe(newPos);
	m_fPosOnMainPipe = ClipToMinMax(m_fPosOnMainPipe, 0.0f, 1.0f);
	RecalcPositions();
}

void Synapse::RecalcPositions()
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

	m_fDirection      = (umCrit < 0._MicroMeter) ? 1.0f : -1.0f;
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
	if (m_bOutputBlocked)
		wcout << L"blocked since " << m_usBlocked << L"µs" << endl;
}

void Synapse::Reconnect()
{
	m_pPipeAdd->SetEndPnt(this);
	m_pPipeAdd->PosChanged();
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

void Synapse::Apply2AllInPipes(PipeFunc const& f)
{
	f(*m_pPipeMain);
	f(*m_pPipeAdd);
}

void Synapse::Apply2AllOutPipes(PipeFunc const& f)
{
	f(*m_pPipeMain);
}

void Synapse::Apply2AllInPipesC(PipeFuncC const& f) const
{
	f(*m_pPipeMain);
	f(*m_pPipeAdd);
}

void Synapse::Apply2AllOutPipesC(PipeFuncC const& f) const
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
	if (m_bOutputBlocked)
	{
		m_mVinputBuffer = 0.0_mV;
		m_usBlocked += GetParam()->TimeResolution();
		if (m_usBlocked > GetParam()->PulseDistMin())
			m_bOutputBlocked = false;
	}
	else
	{
		m_mVinputBuffer += m_mVaddInput;
		if (m_mVinputBuffer >= GetParam()->SynapseThreshold())
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
