// Pipe.cpp 
//
// NNetModel

module;

#include <cassert>
#include <algorithm>
#include <iostream>
#include <vector>
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
using std::vector;
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
	assert(pKnotStart);
	assert(pKnotEnd);
	RecalcSegments();
}

Pipe::Pipe(Pipe const & src) :  // copy constructor
	Nob        (src),
    m_pNobStart(nullptr),
	m_pNobEnd  (nullptr),
	m_potIndex (src.m_potIndex ),
	m_segments(src.m_segments),
	m_synapses(src.m_synapses)
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
	fill(m_segments, 0.0_mV);
	RecalcSegments();
}

bool Pipe::IsConnectedSynapse(Nob const & synapse) const
{
	for (Nob* nob : m_synapses)
		if (nob->GetId() == synapse.GetId())
			return true;
	return false;
}

void Pipe::Link(Nob const & nobSrc,	Nob2NobFunc const & dstFromSrc)
{
	Pipe const & pipeSrc { static_cast<Pipe const &>(nobSrc) };
	m_pNobStart = static_cast<PosNob *>(dstFromSrc(pipeSrc.GetStartNobPtr()));
	m_pNobEnd   = static_cast<PosNob *>(dstFromSrc(pipeSrc.GetEndNobPtr  ()));
	for (int i = 0; i < m_synapses.size(); ++i)
		m_synapses[i] = static_cast<PosNob*>(dstFromSrc(pipeSrc.m_synapses[i]));
}

void Pipe::Check() const
{
	Nob::Check();
	for (auto it : m_synapses)
		it->Check();
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
	assert(pPosNob);
	m_pNobStart = pPosNob;
}

void Pipe::SetEndPnt(Nob * const pPosNob)  //TODO: Nob --> PosNob
{
	assert(pPosNob);
	m_pNobEnd = pPosNob;
}

void Pipe::DislocateEndPoint() 
{ 
	m_pNobEnd->MoveNob(-dislocation());
}

void Pipe::DislocateStartPoint() 
{ 
	m_pNobStart->MoveNob(dislocation());
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

MicroMeter Pipe::DistPntToPipe(MicroMeterPnt const& umPoint) const
{
	return PointToLine
	(
		m_pNobStart->GetPos(),
		m_pNobEnd->GetPos(),
		umPoint
	);
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

void Pipe::CreateSynapse(Nob* pNob)
{
	assert(pNob->IsSynapse());
	m_synapses.push_back(pNob);
}

void Pipe::SetNrOfSegments(size_t const n) const
{
	m_segments.resize(n, 0.0_mV);
	m_bSegmentsDirty = false;
	m_potIndex = 0;
}

void Pipe::recalcSegments() const
{
	meterPerSec  const pulseSpeed    { meterPerSec(GetParam()->GetParameterValue(ParamType::Value::pulseSpeed)) };
	MicroMeter   const segmentLength { CoveredDistance(pulseSpeed, GetParam()->TimeResolution()) };
	unsigned int const iNrOfSegments { max(1U, Cast2UnsignedInt(round(GetLength() / segmentLength))) };
	SetNrOfSegments(iNrOfSegments);
}

MicroMeterPnt Pipe::getSegmentPos(SegNr const segNr, float const fPos) const
{
	MicroMeterPnt const umVector { GetEndPoint() - GetStartPoint() };
	MicroMeterPnt const umpSegVec { umVector / Cast2Float(GetNrOfSegments()) };
	float         const fPosition { (static_cast<float>(segNr.GetValue()) + fPos) };
	return GetStartPoint() + umpSegVec * fPosition;
}

vector<mV> const& Pipe::getSegments() const
{
	if (m_bSegmentsDirty)
		recalcSegments();
	return m_segments;
}

void Pipe::PositionChanged()
{
	assert(IsPipe());
	posChangedRecursive(*this);
}

void Pipe::posChangedRecursive(Pipe const& pipeOrigin)
{
	if (m_pNobStart && m_pNobEnd)
	{
		Apply2AllSynapses
		(
			[this, &pipeOrigin](Nob* pNob)
			{
				Synapse* pSynapse { Cast2Synapse(pNob) };
				Pipe   * pPipeAdd { pSynapse->GetAddPipe() };
				if (pPipeAdd != &pipeOrigin)
					pPipeAdd->posChangedRecursive(pipeOrigin);
				pSynapse->RecalcPositions();
			}
		);
		if (m_pNobEnd->IsSynapse())
			Cast2Synapse(m_pNobEnd)->RecalcPositions();
		RecalcSegments();
	}
}

void Pipe::RemoveSynapse(Nob* pSynapse)
{
	auto res { find(m_synapses, pSynapse) };
	assert(res != end(m_synapses));
	m_synapses.erase(res);
}

MicroMeterPnt Pipe::GetSegmentVector() const
{
	MicroMeterPnt const umVector  { GetEndPoint() - GetStartPoint() };
	MicroMeterPnt const umpSegVec { umVector / Cast2Float(GetNrOfSegments()) };
	return umpSegVec;
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
	m_segments[m_potIndex] = m_mVinputBuffer;
	if (m_potIndex == 0)
		m_potIndex = getSegments().size() - 1;  // caution!
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
				mVresult = getSegments()[index];
				break;
			}
			umPoint = umPoint2;

		} while (index != potIndex);
	}
	return mVresult;
}

void Pipe::AppendMenuItems(AddMenuFunc const & add) const
{
	add(IDD_CREATE_FORK);
	add(IDD_CREATE_SYNAPSE);
	add(IDD_INSERT_NEURON);
	add(IDD_INSERT_KNOT);   
	Nob::AppendMenuItems(add);
	add(IDD_DELETE_NOB);
}

wostream & operator<< (wostream & out, Pipe const & pipe)
{
	out << OPEN_BRACKET 
		<< pipe.GetStartKnotId() 
		<< PIPE_TO
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
