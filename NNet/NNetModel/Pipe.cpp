// Pipe.cpp 
//
// NNetModel

#include "stdafx.h"
#include <assert.h>
#include "Resource.h"
#include "DrawContext.h"
#include "NNetParameters.h"
#include "BaseKnot.h"
#include "Knot.h"
#include "Pipe.h"

using std::wcout;
using std::endl;
using std::ranges::fill;

Pipe::Pipe()
  :	Nob(NobType::Value::pipe)
{}

Pipe::Pipe
(
	BaseKnot * const   pKnotStart, 
	BaseKnot * const   pKnotEnd,
	Param      const & param
)
  :	Nob(NobType::Value::pipe),
	m_pKnotStart(pKnotStart),
	m_pKnotEnd  (pKnotEnd)
{
	assert(pKnotStart && pKnotEnd);
	recalc(param);
}

Pipe::Pipe(Pipe const & src) :  // copy constructor
	Nob         (src),
    m_pKnotStart(nullptr),
	m_pKnotEnd  (nullptr),
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
	return m_pKnotStart->GetId(); 
}

NobId Pipe::GetEndKnotId() const 
{ 
	return m_pKnotEnd->GetId(); 
}

MicroMeterPnt Pipe::GetPos() const 
{ 
	return (m_pKnotStart->GetPos() + m_pKnotEnd->GetPos()) / 2.0f; 
}

void Pipe::ClearDynamicData()
{
	Nob::ClearDynamicData();
	fill(m_potential, 0.0_mV);
}

void Pipe::recalc(Param const & param)
{
	meterPerSec  const pulseSpeed    { meterPerSec(param.GetParameterValue(ParamType::Value::pulseSpeed)) };
	MicroMeter   const segmentLength { CoveredDistance(pulseSpeed, param.TimeResolution()) };
	MicroMeter   const pipeLength    { Distance(m_pKnotStart->GetPos(), m_pKnotEnd->GetPos()) };
	unsigned int const iNrOfSegments { max(1, Cast2UnsignedInt(round(pipeLength / segmentLength))) };
	m_potential.resize(iNrOfSegments, 0.0_mV);
	m_potIndex = 0;
}

void Pipe::Recalc()
{
	if (m_pKnotStart && m_pKnotEnd)
	{
		recalc(* m_pParameters);
	}
}

void Pipe::Link(Nob const & nobSrc,	Nob2NobFunc const & dstFromSrc)
{
	Pipe const & pipeSrc { static_cast<Pipe const &>(nobSrc) };
	m_pKnotStart = static_cast<BaseKnot *>(dstFromSrc(pipeSrc.GetStartKnotPtr()));
	m_pKnotEnd   = static_cast<BaseKnot *>(dstFromSrc(pipeSrc.GetEndKnotPtr  ()));
}

void Pipe::Check() const
{
	Nob::Check();
	assert(m_pKnotStart->IsPrecursorOf(* this));
	assert(m_pKnotEnd  ->IsSuccessorOf(* this));
}

void Pipe::Expand(MicroMeterRect & umRect) const
{
	umRect.Expand(GetStartPoint());
	umRect.Expand(GetEndPoint  ());
}

void Pipe::Prepare()
{
	m_mVinputBuffer = m_pKnotStart->GetNextOutput();
}

void Pipe::MoveNob(MicroMeterPnt const & delta)
{
	if (!m_pKnotStart->HasParentNob())
		m_pKnotStart->MoveNob(delta);
	if (!m_pKnotEnd->HasParentNob())
		m_pKnotEnd  ->MoveNob(delta);
}

// IsIncludedIn should be called IsPossiblyIncludedIn
// It doesn't calculate exactly if the pipe intersects umRect, but eliminites a lot of cases with a 
// simple and fast check. The rest is left over for the clipping algorithm of the graphics subsystem

bool Pipe::IsIncludedIn(MicroMeterRect const & umRect) const 
{ 
	if ((m_pKnotStart->GetPosX() < umRect.GetLeft()) && (m_pKnotEnd->GetPosX() < umRect.GetLeft()))
		return false;

	if ((m_pKnotStart->GetPosX() > umRect.GetRight()) && (m_pKnotEnd->GetPosX() > umRect.GetRight()))
		return false;

	if ((m_pKnotStart->GetPosY() > umRect.GetBottom()) && (m_pKnotEnd->GetPosY() > umRect.GetBottom()))
		return false;

	if ((m_pKnotStart->GetPosY() < umRect.GetTop()) && (m_pKnotEnd->GetPosY() < umRect.GetTop()))
		return false;

	return true;
}

void Pipe::SetStartKnot(BaseKnot * const pBaseKnot)
{
	if (pBaseKnot)
	{
		m_pKnotStart = pBaseKnot;
		Recalc();
	}
}

void Pipe::SetEndKnot(BaseKnot * const pBaseKnot)
{
	if (pBaseKnot)
	{
		m_pKnotEnd = pBaseKnot;
		Recalc();
	}
}

void Pipe::DislocateEndPoint() 
{ 
	m_pKnotEnd->MoveNob(-dislocation());
	Recalc();
}

void Pipe::DislocateStartPoint() 
{ 
	m_pKnotStart->MoveNob(dislocation());
	Recalc();
}

MicroMeterPnt Pipe::dislocation() const
{ 
	return GetVector().ScaledTo(PIPE_WIDTH*5);
}

MicroMeterPnt Pipe::GetStartPoint() const 
{ 
	return m_pKnotStart ? m_pKnotStart->GetPos() : MicroMeterPnt::NULL_VAL(); 
}

MicroMeterPnt Pipe::GetEndPoint() const 
{ 
	return m_pKnotEnd ? m_pKnotEnd->GetPos() : MicroMeterPnt::NULL_VAL();
}

void Pipe::Select(bool const bOn) 
{ 
	Nob::Select(bOn);
	if (m_pKnotStart->IsKnot())
		static_cast<Knot *>(m_pKnotStart)->EvaluateSelectionStatus();
	if (m_pKnotEnd  ->IsKnot())
		static_cast<Knot *>(m_pKnotEnd)  ->EvaluateSelectionStatus();
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
	MicroMeterPnt const umOrthoScaled{ umVector.OrthoVector().ScaledTo(PIPE_WIDTH) };
	MicroMeterPnt       umPoint1     { GetStartPoint() };
	MicroMeterPnt const umPoint2     { umPoint1 + umVector };
	return IsPointInRect2<MicroMeterPnt>(point, umPoint1, umPoint2, umOrthoScaled);
}

MicroMeterPnt Pipe::GetVector() const
{
	assert(m_pKnotStart);
	assert(m_pKnotEnd);
	MicroMeterPnt const umStartPoint { GetStartPoint() };
	MicroMeterPnt const umEndPoint   { GetEndPoint  () };
	MicroMeterPnt const umVector     { umEndPoint - umStartPoint };
	assert(! umVector.IsCloseToZero());
	return umVector;
}

MicroMeterPnt Pipe::GetVector(float const fFactor) const
{
	assert(m_pKnotStart);
	assert(m_pKnotEnd);
	MicroMeterPnt const umStartPoint { GetStartPoint() };
	MicroMeterPnt const umEndPoint   { GetEndPoint  () };
	MicroMeterPnt const umVector     { umEndPoint - umStartPoint };
	MicroMeterPnt const umResult     { umStartPoint + umVector * fFactor};
	assert(! umResult.IsCloseToZero());
	return umResult;
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
		(umEndPoint * 2.f + umStartPoint) / 3.f , 
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
	if (!bDownStream && m_pKnotStart->IsKnot())
		static_cast<Knot *>(m_pKnotStart)->Emphasize(bOn, false);
	if (bDownStream && m_pKnotEnd->IsKnot())
		static_cast<Knot *>(m_pKnotEnd)->Emphasize(bOn, true);
}

void Pipe::Emphasize(bool const bOn) 
{ 
	Nob::Emphasize(bOn); 
	if (m_pKnotStart->IsKnot())
		static_cast<Knot *>(m_pKnotStart)->Emphasize(bOn, false);
	if (m_pKnotEnd->IsKnot())
		static_cast<Knot *>(m_pKnotEnd)->Emphasize(bOn, true);
}

size_t Pipe::segNr2index(SegNr const segNr) const
{
	size_t index { segNr.GetValue() + m_potIndex };
	if (index >= GetNrOfSegments())
		index -= GetNrOfSegments();
	return index;
}
