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
    m_observable.RegisterObserver(*this);
}

Signal::~Signal()
{
    m_observable.UnregisterObserver(*this);
}

void Signal::add2list(Pipe const & pipe) 
{  
    float const DATA_PNTS { 10.0f };
    float const fIncCalc  { m_circle.GetRadius() / (pipe.GetLength() * DATA_PNTS) };
    float const fInc      { min(1.0f, fIncCalc) };
    for (float fRun = 0.0f; fRun <= 1.0f; fRun += fInc)
    {
        MicroMeterPnt umpRun  { pipe.GetVector(fRun) };
        float         fFactor { GetDistFactor(umpRun) };
        if (fFactor > 0.0f)
        {
            m_dataPoints.push_back(SigDataPoint(&pipe, pipe.GetSegNr(fRun), umpRun, fFactor));
        }
    }
} 

void Signal::Recalc()
{
    m_dataPoints.clear();
    m_nmri.GetUPNobsC().Apply2All<Pipe>([this](Pipe const & pipe) { add2list(pipe); });
}

float Signal::GetSignalValue() const
{
    float fResult { 0.0f };
    for (auto const & it : m_dataPoints)
        fResult += it.GetSignalValue();
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

void Signal::DrawDataPoints(DrawContext const & context) const
{
    for (auto const& it : m_dataPoints)
    {
        mV     const voltage { it.m_pPipe->GetVoltage(it.m_segNr) };
        ColorF const col     { it.m_pPipe->GetInteriorColor(voltage) };
        context.FillCircle(it.dataPointCircle(), col);
    }
}

float Signal::GetDistFactor(MicroMeterPnt const & umPnt) const
{
    return m_circle.DistFactor(umPnt);
}

Signal::SigDataPoint const * Signal::findDataPoint(MicroMeterPnt const & umPnt) const
{
    for (auto const & it : m_dataPoints)
        if (it.dataPointCircle().Includes(umPnt))
            return &it;
    return nullptr;
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

void Signal::SetSensorPos(MicroMeterPnt const & umPos) 
{ 
    m_circle.SetPos(umPos); 
    Recalc();
}

void Signal::SetSensorSize(MicroMeter const umSize) 
{ 
    m_circle.SetRadius(umSize); 
    m_fDsBorder = umSize.GetValue() * umSize.GetValue();
    Recalc();
}

void Signal::MoveSensor(MicroMeterPnt const & umDelta) 
{ 
    SetSensorPos(m_circle.GetPos() + umDelta);
}

void Signal::SizeSensor(float const factor) 
{ 
    SetSensorSize(GetRadius() * factor); 
}

void Signal::RotateSensor(MicroMeterPnt const & umPntPivot, Radian const radDelta)
{
    m_circle.Rotate(umPntPivot, radDelta);
}
