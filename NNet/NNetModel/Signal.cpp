// Signal.cpp 
//
// NNetModel

#include "stdafx.h"
#include "DrawContext.h"
#include "UPNobList.h"
#include "NNetColors.h"
#include "BaseKnot.h"
#include "Signal.h"

using std::wcout;
using std::endl;
using std::make_unique;

Signal::Signal
(
    Observable             & observable,
    UPNobList        const & list,
    MicroMeterCircle const & circle
) :
    m_observable(observable),
    m_circle(circle)
{
    m_upMeanFilter = make_unique<MeanFilter>();
    SetSensorSize(list, circle.GetRadius());
    m_observable.RegisterObserver(*this);
}

Signal::~Signal()
{
    m_observable.UnregisterObserver(*this);
}

void Signal::Reset(UPNobList const & list)    
{ 
    m_timeStart = 0.0_MicroSecs;
    m_upMeanFilter->Reset(); 
    Recalc(list); 
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

void Signal::Recalc(UPNobList const & list)
{
    m_dataPoints.clear();
    list.Apply2AllC<Pipe>([this](Pipe const & pipe) { add2list(pipe); });
    m_upMeanFilter->Recalc();
}

void Signal::RecalcFilter(Param const & param) const
{
    fMicroSecs usTimeRes    { param.TimeResolution() };
    fMicroSecs usFilterSize { param.FilterSize() };
    size_t     filterSize   { static_cast<size_t>(usFilterSize/usTimeRes) };
    m_upMeanFilter->SetFilterSize(filterSize);
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

SIG_INDEX Signal::time2index
(
    Param      const & param,
    fMicroSecs const   usParam
) const
{
    assert(usParam >= m_timeStart);
    fMicroSecs const timeTilStart { usParam - m_timeStart };
    float      const fNrOfPoints  { timeTilStart / param.TimeResolution() };
    SIG_INDEX  const index        { static_cast<SIG_INDEX>(roundf(fNrOfPoints)) };
    return index;
}

fMicroSecs Signal::index2time
(
    Param     const & param,
    SIG_INDEX const   index
) const
{
    if (index < 0)
        return 0.0_MicroSecs;
    float      const fNrOfPoints  { static_cast<float>(index) };
    fMicroSecs const timeTilStart { param.TimeResolution() * fNrOfPoints };
    fMicroSecs const usResult     { timeTilStart + m_timeStart };
    return usResult;
}

float Signal::GetFilteredDataPoint
(
    Param      const & param,
    fMicroSecs const   time
) const
{
    SIG_INDEX index        { time2index(param, time) };
    size_t    nrOfElements { m_upMeanFilter->GetNrOfElements() };
    if (index >= nrOfElements)
        index = INVALID_SIG_INDEX;
    return m_upMeanFilter->GetFiltered(index);
}

float Signal::GetRawDataPoint
(
    Param      const & param,
    fMicroSecs const   time
) const
{
    SIG_INDEX index { time2index(param, time) };
    return m_upMeanFilter->GetRaw(index);
}

fMicroSecs Signal::FindNextMaximum
(
    Param      const & param,
    fMicroSecs const   time
) const
{
    SIG_INDEX index { time2index(param, time) };
    size_t           nrOfElements { m_upMeanFilter->GetNrOfElements() };
    if (index >= nrOfElements)
        index = INVALID_SIG_INDEX;
    if ((index > 0) && (m_upMeanFilter->GetFiltered(index-1) > m_upMeanFilter->GetFiltered(index))) // falling values, go left
    {   
        while ((--index > 0) && (m_upMeanFilter->GetFiltered(index-1) >= m_upMeanFilter->GetFiltered(index)));
    }
    else   // climbing values, go right
    {
        while ((index < m_upMeanFilter->GetLastIndex()) && (m_upMeanFilter->GetFiltered(index) <= m_upMeanFilter->GetFiltered(index+1)))
            ++index;
    }
    return index2time(param, index);
}

void Signal::Notify(bool const bImmediate)
{
    m_upMeanFilter->Add(GetSignalValue());
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
    m_upMeanFilter->Dump();
}

void Signal::SetSensorPos
(
    UPNobList     const & list,
    MicroMeterPnt const & umPos
) 
{ 
    m_circle.SetPos(umPos); 
    Recalc(list);
}

void Signal::SetSensorSize
(
    UPNobList  const & list,
    MicroMeter const   umSize
) 
{ 
    m_circle.SetRadius(umSize); 
    m_fDsBorder = umSize.GetValue() * umSize.GetValue();
    Recalc(list);
}

void Signal::MoveSensor
(
    UPNobList     const & list,
    MicroMeterPnt const & umDelta
) 
{ 
    SetSensorPos(list, m_circle.GetPos() + umDelta);
}

void Signal::SizeSensor
(
    UPNobList const & list,
    float     const   factor
) 
{ 
    SetSensorSize(list, GetRadius() * factor); 
}

void Signal::RotateSensor
(
    MicroMeterPnt const & umPntPivot, 
    Radian        const   radDelta
)
{
    m_circle.Rotate(umPntPivot, radDelta);
}
