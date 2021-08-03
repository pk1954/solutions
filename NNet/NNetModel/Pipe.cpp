// Pipe.cpp 
//
// NNetModel

#include "stdafx.h"
#include <cmath>
#include <assert.h>
#include "Geometry.h"
#include "DrawContext.h"
#include "tHighlightType.h"
#include "NNetParameters.h"
#include "BaseKnot.h"
#include "Knot.h"
#include "Pipe.h"

using std::fixed;
using std::wstring;
using std::wostringstream;

#include <sstream> 
#include <iomanip>
using std::fill;
using std::wcout;
using std::endl;

Pipe::Pipe(BaseKnot * const pKnotStart, BaseKnot * const pKnotEnd)
  :	Nob(NobType::Value::pipe),
	m_pKnotStart(pKnotStart),
	m_pKnotEnd  (pKnotEnd)
{
	assert(pKnotStart && pKnotEnd);
	Recalc();
}

Pipe::Pipe(Pipe const & src) :  // copy constructor
	Nob         (src),
    m_pKnotStart(nullptr),
	m_pKnotEnd  (nullptr),
	m_potIndex  (src.m_potIndex ),
	m_potential (src.m_potential)
{ }

Pipe::~Pipe() { }

void Pipe::Dump() const
{
	Nob::Dump();
	wcout << L' ' << * this << endl;
}

void Pipe::init(const Pipe & rhs)
{
	Nob::operator=(rhs);
	m_pKnotStart = nullptr;
	m_pKnotEnd   = nullptr;
	m_potIndex   = rhs.m_potIndex;
	m_potential  = rhs.m_potential;
}

bool Pipe::operator==(Nob const & rhs) const 
{
	Pipe const & pipeRhs { static_cast<Pipe const &>(rhs) };
	return
	(this->Nob::operator== (rhs))                          && 
	(m_pKnotStart->GetId() == pipeRhs.m_pKnotStart->GetId()) &&
	(m_pKnotEnd  ->GetId() == pipeRhs.m_pKnotEnd  ->GetId());
}

NobId Pipe::GetStartKnotId() const 
{ 
	return m_pKnotStart->GetId(); 
}

NobId Pipe::GetEndKnotId() const 
{ 
	return m_pKnotEnd->GetId(); 
}

MicroMeterPnt const Pipe::GetPos() const 
{ 
	return (m_pKnotStart->GetPos() + m_pKnotEnd->GetPos()) / 2.0f; 
}

void Pipe::Clear()
{
	Nob::Clear();
	fill(m_potential.begin(), m_potential.end(), 0.0_mV);
}

void Pipe::Recalc()
{
	if (m_pKnotStart && m_pKnotEnd)
	{
		meterPerSec  const pulseSpeed    { meterPerSec(m_pParameters->GetParameterValue(ParamType::Value::pulseSpeed)) };
		MicroMeter   const segmentLength { CoveredDistance(pulseSpeed, m_pParameters->GetTimeResolution()) };
		MicroMeter   const pipeLength    { Distance(m_pKnotStart->GetPos(), m_pKnotEnd->GetPos()) };
		unsigned int const iNrOfSegments { max(1, Cast2UnsignedInt(round(pipeLength / segmentLength))) };
		m_potential.resize(iNrOfSegments, BASE_POTENTIAL);
		m_potIndex = 0;
	}
}
void Pipe::Link(Nob const & nobSrc,	Nob2NobFunc const & dstFromSrc)
{
	Pipe const & pipeSrc { static_cast<Pipe const &>(nobSrc) };
	BaseKnot * const pBaseKnotStart { static_cast<BaseKnot *>(dstFromSrc(pipeSrc.GetStartKnotPtr())) };
	BaseKnot * const pBaseKnotEnd   { static_cast<BaseKnot *>(dstFromSrc(pipeSrc.GetEndKnotPtr  ())) };
	SetStartKnot(pBaseKnotStart);
	SetEndKnot  (pBaseKnotEnd);
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
	m_pKnotStart->MoveNob(delta);
	m_pKnotEnd  ->MoveNob(delta);
}

// IsIncludedIn should be called IsPossiblyIncludedIn
// It doesn't calculate exactly if the pipe intersects umRect, but eliminites a lot of cases with a 
// simple and fast check. The rest is left over for the clipping algorithm of the graphics subsystem

bool const Pipe::IsIncludedIn(MicroMeterRect const & umRect) const 
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
	assert(pBaseKnot);
	m_pKnotStart = pBaseKnot;
	Recalc();
}

void Pipe::SetEndKnot(BaseKnot * const pBaseKnot)
{
	assert(pBaseKnot);
	m_pKnotEnd = pBaseKnot;
	Recalc();
}

void Pipe::dislocate(BaseKnot * const pBaseKnot, MicroMeter const dislocation)
{ 
	pBaseKnot->MoveNob(GetVector().OrthoVector().ScaledTo(dislocation));
	Recalc();
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

bool const Pipe::Includes(MicroMeterPnt const & point) const
{
	MicroMeterPnt const umVector{ GetEndPoint() - GetStartPoint() };
	if (umVector.IsCloseToZero())
		return false;
	MicroMeterPnt const umOrthoScaled{ umVector.OrthoVector().ScaledTo(PIPE_WIDTH) };
	MicroMeterPnt       umPoint1     { GetStartPoint() };
	MicroMeterPnt const umPoint2     { umPoint1 + umVector };
	return IsPointInRect2<MicroMeterPnt>(point, umPoint1, umPoint2, umOrthoScaled);
}

MicroMeterPnt Pipe::GetVector() const
{
	MicroMeterPnt const umStartPoint { GetStartPoint() };
	MicroMeterPnt const umEndPoint   { GetEndPoint  () };
	MicroMeterPnt const umvector{ umEndPoint - umStartPoint };
	assert(! umvector.IsCloseToZero());
	return umvector;
}

void Pipe::DrawArrows
(
	DrawContext const & context, 
	MicroMeter  const   umSize
) const
{
	MicroMeterPnt const umStartPoint { GetStartPoint() };
	MicroMeterPnt const umEndPoint   { GetEndPoint  () };

	context.FillArrow
	(
		(umEndPoint * 2.f + umStartPoint) / 3.f , 
		umEndPoint - umStartPoint, 
		umSize,
		PIPE_WIDTH / 2, 
		NNetColors::EXT_NORMAL
	);
}

void Pipe::DrawExterior(DrawContext const & context, tHighlight const type) const
{
	context.DrawLine(GetStartPoint(), GetEndPoint(), PIPE_WIDTH, GetExteriorColor(type));
}

void Pipe::DrawInterior(DrawContext const & context, tHighlight const type) const
{
	static MicroMeter const umWidth { PIPE_WIDTH * PIPE_INTERIOR };

	if (IsNormal(type) && ! IsSelected())
	{
		MicroMeterPnt const umVector { GetEndPoint() - GetStartPoint() };
		if (! umVector.IsCloseToZero())
		{
			size_t          const nrOfSegments { m_potential.size() };
			MicroMeterPnt const umSegVec     { umVector / Cast2Float(nrOfSegments) };
			MicroMeterPnt       umPoint      { GetStartPoint() };
			size_t          const potIndex     { m_potIndex };
			size_t                index        { potIndex }; 
			do 
			{
				if (++index == m_potential.size()) 
					index = 0; 

				MicroMeterPnt const umPointNext { umPoint + umSegVec };
				context.DrawLine(umPoint, umPointNext, umWidth, GetInteriorColor(m_potential[index]));
				umPoint = umPointNext;
			} while (index != potIndex);
		}
	}
	else
	{
		context.DrawLine(GetStartPoint(), GetEndPoint(), umWidth, GetInteriorColor(type));
	}
}

bool const Pipe::CompStep()
{
	m_potential[m_potIndex] = m_mVinputBuffer;
	if (m_potIndex == 0)
		m_potIndex = m_potential.size() - 1;  // caution!
	else                                      // modification if m_potIndex
		--m_potIndex;                         // must be atomic
	return false;
}

mV const Pipe::GetVoltage(MicroMeterPnt const & point) const
{
	mV mVresult { 0._mV };
	MicroMeterPnt const umVector { GetEndPoint() - GetStartPoint() };
	if (! umVector.IsCloseToZero())
	{
		size_t          const nrOfSegments  { m_potential.size() };
		MicroMeterPnt const umSegVec      { umVector / Cast2Float(nrOfSegments) };
		MicroMeterPnt const umOrthoScaled { umVector.OrthoVector().ScaledTo(PIPE_WIDTH) };
		MicroMeterPnt       umPoint       { GetStartPoint() };
		size_t          const potIndex      { m_potIndex };
		size_t                index         { potIndex }; 
		do 
		{
			if (++index == m_potential.size()) 
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
	out << Pipe::OPEN_BRACKET 
		<< pipe.GetStartKnotId() 
		<< Pipe::SEPARATOR
		<< pipe.GetEndKnotId() 
		<< Pipe::CLOSE_BRACKET;
	return out;
}
