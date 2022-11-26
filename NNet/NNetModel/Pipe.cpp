// Pipe.cpp 
//
// NNetModel

module;

#include <cassert>
#include <algorithm>
#include <iostream>
#include "Resource.h"

module NNetModel:Pipe;

import Geometry;
import IoConstants;
import Types;
import DrawContext;
import :tHighlight;
import :NNetParameters;
import :Knot;
import :Synapse;
import :PosNob;
import :NobType;
import :NobId;
import :Nob;

using std::max;
using std::wcout;
using std::wostream;
using std::endl;
using std::ranges::fill;
using std::ranges::find;
using std::unique_ptr;
using std::make_unique;
using std::pair;

Pipe::Pipe()
  :	Nob(NobType::Value::pipe)
{}

Pipe::Pipe
(
	Nob * const pKnotStart, //TODO: Nob --> PosNob
	Nob * const pKnotEnd    //TODO: Nob --> PosNob
)
  :	Nob(NobType::Value::pipe),
	m_pNobStart(pKnotStart),
	m_pNobEnd  (pKnotEnd)
{
	assert(pKnotStart && pKnotEnd);
	recalc();
}

Pipe::Pipe(Pipe const & src) :  // copy constructor
	Nob         (src),
    m_pNobStart(nullptr),
	m_pNobEnd  (nullptr),
	m_potIndex  (src.m_potIndex ),
	m_potential (src.m_potential)
{ 
}

void Pipe::Dump() const
{
	Nob::Dump();
	wcout << L' ' << * this << endl;
}

bool Pipe::operator==(Nob const & rhs) const 
{
	Pipe const & pipeRhs { static_cast<Pipe const &>(rhs) };
	return
	(this->Nob::operator==(rhs))                   && 
	(GetStartKnotId() == pipeRhs.GetStartKnotId()) &&
	(GetEndKnotId  () == pipeRhs.GetEndKnotId());
}

NobId Pipe::GetStartKnotId() const 
{ 
	return m_pNobStart->GetId(); 
}

NobId Pipe::GetEndKnotId() const 
{ 
	return m_pNobEnd->GetId(); 
}

MicroMeterPnt Pipe::GetPos() const 
{ 
	return (m_pNobStart->GetPos() + m_pNobEnd->GetPos()) / 2.0f; 
}

void Pipe::ClearDynamicData()
{
	Nob::ClearDynamicData();
	fill(m_potential, 0.0_mV);
}

void Pipe::recalc()
{
	meterPerSec  const pulseSpeed    { meterPerSec(GetParam()->GetParameterValue(ParamType::Value::pulseSpeed)) };
	MicroMeter   const segmentLength { CoveredDistance(pulseSpeed, GetParam()->TimeResolution()) };
	MicroMeter   const pipeLength    { Distance(m_pNobStart->GetPos(), m_pNobEnd->GetPos()) };
	unsigned int const iNrOfSegments { max(1U, Cast2UnsignedInt(round(pipeLength / segmentLength))) };
	m_potential.resize(iNrOfSegments, 0.0_mV);
	m_potIndex = 0;
}

void Pipe::Recalc()
{
	if (m_pNobStart && m_pNobEnd)
		recalc();
}

bool Pipe::IsConnectedSynapse(Nob const & synapse) const
{
	return find(m_synapses, &synapse) != end(m_synapses);
}

void Pipe::Link(Nob const & nobSrc,	Nob2NobFunc const & dstFromSrc)
{
	Pipe const & pipeSrc { static_cast<Pipe const &>(nobSrc) };
	m_pNobStart = static_cast<PosNob *>(dstFromSrc(pipeSrc.GetStartNobPtr()));
	m_pNobEnd   = static_cast<PosNob *>(dstFromSrc(pipeSrc.GetEndNobPtr  ()));
}

void Pipe::Check() const
{
	Nob::Check();
	//assert(static_cast<PosNob *>(m_pNobStart)->IsPrecursorOf(* this));
	//assert(static_cast<PosNob *>(m_pNobEnd  )->IsSuccessorOf(* this));
}

void Pipe::Expand(MicroMeterRect & umRect) const
{
	umRect.Expand(GetStartPoint());
	umRect.Expand(GetEndPoint  ());
}

void Pipe::MoveNob(MicroMeterPnt const & delta)
{
	if (!m_pNobStart->HasParentNob())
		m_pNobStart->MoveNob(delta);
	if (!m_pNobEnd->HasParentNob())
		m_pNobEnd  ->MoveNob(delta);
}

// IsIncludedIn should be called IsPossiblyIncludedIn
// It doesn't calculate exactly if the pipe intersects umRect, but eliminites a lot of cases with a 
// simple and fast check. The rest is left over for the clipping algorithm of the graphics subsystem

bool Pipe::IsIncludedIn(MicroMeterRect const & umRect) const 
{ 
	if ((m_pNobStart->GetPosX() < umRect.GetLeft()) && (m_pNobEnd->GetPosX() < umRect.GetLeft()))
		return false;

	if ((m_pNobStart->GetPosX() > umRect.GetRight()) && (m_pNobEnd->GetPosX() > umRect.GetRight()))
		return false;

	if ((m_pNobStart->GetPosY() > umRect.GetBottom()) && (m_pNobEnd->GetPosY() > umRect.GetBottom()))
		return false;

	if ((m_pNobStart->GetPosY() < umRect.GetTop()) && (m_pNobEnd->GetPosY() < umRect.GetTop()))
		return false;

	return true;
}

void Pipe::SetStartPnt(Nob * const pPosNob)  //TODO: Nob --> PosNob
{
	if (pPosNob)
	{
		m_pNobStart = pPosNob;
		Recalc();
	}
}

void Pipe::SetEndPnt(Nob * const pPosNob)  //TODO: Nob --> PosNob
{
	if (pPosNob)
	{
		m_pNobEnd = pPosNob;
		Recalc();
	}
}

void Pipe::DislocateEndPoint() 
{ 
	m_pNobEnd->MoveNob(-dislocation());
	Recalc();
}

void Pipe::DislocateStartPoint() 
{ 
	m_pNobStart->MoveNob(dislocation());
	Recalc();
}

MicroMeterPnt Pipe::dislocation() const
{ 
	return GetVector().ScaledTo(PIPE_WIDTH*5);
}

MicroMeterPnt Pipe::GetStartPoint() const 
{ 
	return m_pNobStart ? m_pNobStart->GetPos() : MicroMeterPnt::NULL_VAL(); 
}

MicroMeterPnt Pipe::GetEndPoint() const 
{ 
	return m_pNobEnd ? m_pNobEnd->GetPos() : MicroMeterPnt::NULL_VAL();
}

void Pipe::Select(bool const bOn) 
{ 
	Nob::Select(bOn);
	if (m_pNobStart->IsKnot())
		static_cast<PosNob *>(m_pNobStart)->EvaluateSelectionStatus();
	if (m_pNobEnd  ->IsKnot())
		static_cast<PosNob *>(m_pNobEnd)  ->EvaluateSelectionStatus();
}

MicroMeter Pipe::GetLength() const
{
	return Distance(GetStartPoint(), GetEndPoint());
}

bool Pipe::Includes(MicroMeterPnt const & point) const
{
	MicroMeterPnt const umVector{ GetVector() };
	if (umVector.IsCloseToZero())
		return false;
	MicroMeterPnt const umOrthoScaled { umVector.OrthoVector().ScaledTo(PIPE_WIDTH) };
	MicroMeterPnt const umPoint1      { GetStartPoint() };
	MicroMeterPnt const umPoint2      { umPoint1 + umVector };
	return IsPointInRect2<MicroMeterPnt>(point, umPoint1, umPoint2, umOrthoScaled);
}

bool Pipe::IsConnectedTo(NobId const idPosNob) const
{
	return (GetStartKnotId() == idPosNob) || (GetEndKnotId() == idPosNob);
}

MicroMeterPnt Pipe::GetVector() const
{
	assert(m_pNobStart);
	assert(m_pNobEnd);
	MicroMeterPnt const umVector { m_pNobEnd->GetPos() - GetStartPoint() };
	assert(! umVector.IsCloseToZero());
	return umVector;
}

// Split: create two new pipes and add links to Synapses of existing Pipe
//        do not affect Synapses themselves

pair<unique_ptr<Pipe>, unique_ptr<Pipe>> Pipe::Split(Nob & nobSplit) const
{
	unique_ptr<Pipe> upPipe1   { make_unique<Pipe>(m_pNobStart, &nobSplit) };
	unique_ptr<Pipe> upPipe2   { make_unique<Pipe>(&nobSplit, m_pNobEnd) };
	float            fPosSplit { PosOnPipe(nobSplit.GetPos()) };

	for (auto it : m_synapses)
	{
		Synapse &    synapse  { static_cast<Synapse&>(*it) };
		float  const fPos     { synapse.GetPosOnMainPipe() };
		Pipe * const pPipeNew { (fPos < fPosSplit) ? upPipe1.get() : upPipe2.get() };
		pPipeNew->m_synapses.push_back(&synapse);
	}

	nobSplit.Select(IsSelected());
	upPipe1->Select(IsSelected());
	upPipe2->Select(IsSelected());

	return pair(move(upPipe1), move(upPipe2));
}

void Pipe::AddSynapse(Nob* pNob)
{
	assert(pNob->IsSynapse());
	Synapse* pSynapse { static_cast<Synapse*>(pNob) };
	pSynapse->SetMainPipe(this);
	m_synapses.push_back(pSynapse);
}

void Pipe::RemoveSynapse(Nob* pSynapse)
{
	auto res { find(m_synapses, pSynapse) };
	assert(res != end(m_synapses));
	m_synapses.erase(res);
}

MicroMeterPnt Pipe::GetVector(float const fFactor) const
{
	assert(m_pNobStart);
	assert(m_pNobEnd);
	MicroMeterPnt const umResult { GetStartPoint() + GetVector() * fFactor};
	assert(! umResult.IsCloseToZero());
	return umResult;
}

float Pipe::PosOnPipe(MicroMeterPnt const& umPnt) const
{
	float fResult;
	MicroMeterPnt const umPntVector { GetVector() };
	if (umPntVector.GetX().GetAbsValue() > umPntVector.GetY().GetAbsValue())
	{
		fResult = (umPnt.GetX() - GetStartPoint().GetX()) / umPntVector.GetX();
	}
	else
	{
		fResult = (umPnt.GetY() - GetStartPoint().GetY()) / umPntVector.GetY();
	}
	return fResult;
}

void Pipe::FixSynapses() const
{
	for (auto it : m_synapses)
		static_cast<Synapse*>(it)->GetAddPipe().SetEndPnt(it);
}

void Pipe::DrawArrows
(
	DrawContext const & context, 
	MicroMeter  const   umSize
) const
{
	MicroMeterPnt const umStartPoint { GetStartPoint() };
	MicroMeterPnt const umEndPoint   { GetEndPoint  () };
	MicroMeter          umArrowSize  { umSize };
	MicroMeter          umArrowWidth { PIPE_WIDTH / 2 };
	if (IsEmphasized())
	{
		umArrowSize  *= 2.f;
		umArrowWidth *= 2.f;
	}

	context.FillArrow
	(
		(umEndPoint * 2.f + umStartPoint) / 3.f, 
		umEndPoint - umStartPoint, 
		umArrowSize,
		umArrowWidth, 
		GetExteriorColor(tHighlight::normal)
	);
}

void Pipe::DrawExterior(DrawContext const & context, tHighlight const type) const
{
	MicroMeter umWidth { PIPE_WIDTH };
	if (IsEmphasized())
		umWidth *= 2.f;
	context.DrawLine(GetStartPoint(), GetEndPoint(), umWidth, GetExteriorColor(type));
}

void Pipe::DrawInterior(DrawContext const & context, tHighlight const type) const
{
	MicroMeter umWidth      { PIPE_WIDTH * PIPE_INTERIOR };
	fPixel     fPixMinWidth { 1._fPixel };
	if (IsEmphasized())
	{
		umWidth     *= 2.f;
		fPixMinWidth = 3.f;
	}

	if ((type != tHighlight::normal) || IsSelected())
	{
		context.DrawLine(GetStartPoint(), GetEndPoint(), umWidth, GetInteriorColor(type), fPixMinWidth);
	}
	else
	{
		Apply2AllSegments
		(
			[this, &context, umWidth, fPixMinWidth](SegNr const segNr)
			{
				context.DrawLine
				(
					GetSegmentStart(segNr), 
					GetSegmentEnd(segNr), 
					umWidth, 
					GetInteriorColor(GetVoltage(segNr)), 
					fPixMinWidth
				);
			}
		);
	}
}

void Pipe::CollectInput()
{
	m_mVinputBuffer = m_pNobStart->GetNextOutput();
}

bool Pipe::CompStep()
{
	m_potential[m_potIndex] = m_mVinputBuffer;
	if (m_potIndex == 0)
		m_potIndex = m_potential.size() - 1;  // caution!
	else                                      // modification if m_potIndex
		--m_potIndex;                         // must be atomic
	return false;
}

mV Pipe::GetVoltageAt(MicroMeterPnt const & point) const
{
	mV mVresult { 0._mV };
	
	if (MicroMeterPnt const umVector { GetEndPoint() - GetStartPoint() }; ! umVector.IsCloseToZero())
	{
		MicroMeterPnt const umSegVec      { umVector / Cast2Float(GetNrOfSegments()) };
		MicroMeterPnt const umOrthoScaled { umVector.OrthoVector().ScaledTo(PIPE_WIDTH) };
		MicroMeterPnt       umPoint       { GetStartPoint() };
		size_t        const potIndex      { m_potIndex };
		size_t              index         { potIndex }; 
		do 
		{
			if (++index == GetNrOfSegments()) 
				index = 0; 

			MicroMeterPnt const umPoint2 { umPoint + umSegVec };
			if (IsPointInRect2<MicroMeterPnt>(point, umPoint, umPoint2, umOrthoScaled))
			{
				mVresult = m_potential[index];
				break;
			}
			umPoint = umPoint2;

		} while (index != potIndex);
	}
	return mVresult;
}

void Pipe::AppendMenuItems(AddMenuFunc const & add) const
{
	add(IDD_ADD_OUTGOING2PIPE);
	add(IDD_ADD_INCOMING2PIPE);
	add(IDD_INSERT_NEURON);
	add(IDD_INSERT_KNOT);   
	Nob::AppendMenuItems(add);
	add(IDD_DELETE_NOB);
}

Pipe const * Cast2Pipe(Nob const * pNob)
{
	assert(pNob->IsPipe());
	return static_cast<Pipe const *>(pNob);
}

Pipe * Cast2Pipe(Nob * pNob)
{
	assert(pNob->IsPipe());
	return static_cast<Pipe *>(pNob);
}

wostream & operator<< (wostream & out, Pipe const & pipe)
{
	out << OPEN_BRACKET 
		<< pipe.GetStartKnotId() 
		<< FROM_TO
		<< pipe.GetEndKnotId() 
		<< CLOSE_BRACKET;
	return out;
}

void Pipe::Emphasize(bool const bOn, bool bDownStream) 
{ 
	Nob::Emphasize(bOn); 
	if (!bDownStream && m_pNobStart->IsKnot())
		static_cast<Knot *>(m_pNobStart)->Emphasize(bOn, false);
	if (bDownStream && m_pNobEnd->IsKnot())
		static_cast<Knot *>(m_pNobEnd)->Emphasize(bOn, true);
}

void Pipe::Emphasize(bool const bOn) 
{ 
	Nob::Emphasize(bOn); 
	if (m_pNobStart->IsKnot())
		static_cast<Knot *>(m_pNobStart)->Emphasize(bOn, false);
	if (m_pNobEnd->IsKnot())
		static_cast<Knot *>(m_pNobEnd)->Emphasize(bOn, true);
}

size_t Pipe::segNr2index(SegNr const segNr) const
{
	size_t index { segNr.GetValue() + m_potIndex };
	if (index >= GetNrOfSegments())
		index -= GetNrOfSegments();
	return index;
}
