// Signal.cpp 
//
// NNetModel

#include "stdafx.h"
#include "DrawContext.h"
#include "NNetColors.h"
#include "NNetModelReaderInterface.h"
#include "BaseKnot.h"
#include "Signal.h"

using std::wcout;
using std::endl;
using std::make_unique;

Signal::Signal
(
    NNetModelReaderInterface const & nmri,
    Observable                     & observable,
    MicroMeterCircle         const & circle
) :
    m_nmri(nmri),
    m_observable(observable),
    m_circle(circle),
    m_timeStart(nmri.GetSimulationTime())
{
    m_fTimeLine.clear();
    SetSensorSize(circle.GetRadius());
    m_observable.RegisterObserver(this);
}

Signal::~Signal()
{
    m_observable.UnregisterObserver(this);
}

void Signal::add2list(BaseKnot const & baseKnot)
{
    float fDistance { DistSquare(baseKnot.GetPos(), m_circle.GetPos()) };
    if (fDistance < m_fDsBorder)  // is segment in circle?
    {
        float fFactor { 1.0f - fDistance / m_fDsBorder };
        m_baseKnotElements.push_back(BaseKnotElem(&baseKnot, fFactor));
    }
}

void Signal::add2list(Pipe const & pipe) 
{  
    pipe.Apply2AllSegments
    (
        [this, &pipe](Pipe::SegNr const segNr) 
        { 
            MicroMeter const umSegLen { pipe.GetSegLength() };
            float fDistance { DistSquare(pipe.GetSegmentCenter(segNr), m_circle.GetPos()) };
            if (fDistance < m_fDsBorder)  // is segment in circle?
            {
                float fFactor { 1.0f - fDistance / m_fDsBorder };
                m_pipeSegElements.push_back(SegmentElem(&pipe, segNr, fFactor));
            }
        }
    );
} 

void Signal::Recalc()
{
    m_baseKnotElements.clear();
    m_pipeSegElements.clear();
    m_nmri.GetUPNobsC().Apply2All<BaseKnot>([this](BaseKnot const & src ) { add2list(src ); });
    m_nmri.GetUPNobsC().Apply2All<Pipe>    ([this](Pipe     const & pipe) { add2list(pipe); });
}

float Signal::GetSignalValue() const
{
    float fResult { 0.0f };
    for ( auto const& it : m_baseKnotElements )
        fResult += it.m_pBaseKnot->GetVoltage().GetValue() * it.m_fFactor;
    for ( auto const& it : m_pipeSegElements )
        fResult += it.m_pPipe->GetVoltage(it.m_segNr).GetValue() * it.m_fFactor;
    return fResult;
}

void Signal::WriteSignalData(wostream & out) const
{
    out << L"Signal " << m_circle;
}

void Signal::Draw
(
    DrawContext const & context,
    bool        const   bHighlight
) const
{
    context.FillGradientCircle
    (
        m_circle, 
        NNetColors::EEG_SENSOR_1, 
        bHighlight ? NNetColors::EEG_SENSOR_HIGHLIGHTED : NNetColors::EEG_SENSOR_2
    );
}

int Signal::time2index(fMicroSecs const usParam) const
{
    fMicroSecs const timeTilStart { usParam - m_timeStart };
    float      const fNrOfPoints  { timeTilStart / m_nmri.TimeResolution() };
    int              index        { static_cast<int>(roundf(fNrOfPoints)) };
    if (index >= m_fTimeLine.size())
        index = Cast2UnsignedInt(m_fTimeLine.size() - 1);
    return index;
}

fMicroSecs Signal::index2time(int const index) const
{
    float      const fNrOfPoints  { static_cast<float>(index) };
    fMicroSecs const timeTilStart { m_nmri.TimeResolution() * fNrOfPoints };
    fMicroSecs const usResult     { timeTilStart + m_timeStart };
    return usResult;
}

float Signal::GetDataPoint(fMicroSecs const time) const
{
    int index { time2index(time) };
    return (index < 0) ? NAN : m_fTimeLine[index];
}

fMicroSecs Signal::FindNextMaximum(fMicroSecs const time) const
{
    int index { time2index(time) };
    if (index < 0)
        return fMicroSecs::NULL_VAL();
    if ((index > 0) && (m_fTimeLine[index-1] > m_fTimeLine[index])) // falling values, go left
    {   
        while ((--index > 0) && (m_fTimeLine[index-1] >= m_fTimeLine[index]));
    }
    else   // climbing values, go right
    {
        while ((index < m_fTimeLine.size() - 1) && (m_fTimeLine[index] <= m_fTimeLine[index+1]))
            ++index;
    }
    return index2time(index);
}

void Signal::Notify(bool const bImmediate)
{
    m_fTimeLine.push_back(GetSignalValue());
}

void Signal::CheckSignal() const 
{
#ifdef _DEBUG
    if (m_circle.IsNull())
    {
        int x = 42;
    }
#endif
}

void Signal::Dump() const
{
    wcout << L"circle:     " << m_circle << endl;
    wcout << L"time start: " << m_timeStart << endl;
    wcout << '(' << endl;
    for (auto it : m_fTimeLine)
        wcout << it << endl;
    wcout << ')' << endl;
}
