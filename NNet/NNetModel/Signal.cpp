// Signal.cpp 
//
// NNetModel

#include "stdafx.h"
#include "DrawContext.h"
#include "NNetColors.h"
#include "NNetModelReaderInterface.h"
#include "BaseKnot.h"
#include "Pipe.h"
#include "Signal.h"

using std::wcout;
using std::endl;

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
    m_observable.RegisterObserver(this);
}

Signal::~Signal()
{
    m_observable.UnregisterObserver(this);
}

void Signal::recalcSrcList()
{
    float fDsBorder { m_circle.GetRadius().GetValue() * m_circle.GetRadius().GetValue() };
    m_sigSrc.clear();
    m_nmri.GetUPNobsC().Apply2All<BaseKnot>
    (		
        [this, fDsBorder](SignalSource const & sigSrc) 
        {  
            sigSrc.Add2List(m_sigSrc, m_circle.GetPos(), fDsBorder);
        } 
    );
    m_nmri.GetUPNobsC().Apply2All<Pipe>
    (		
        [this, fDsBorder](Pipe const & pipe) 
        {  
            pipe.Apply2AllSegments
            (
                [this, fDsBorder](SignalSource const & sigSrc)
                {
                    sigSrc.Add2List(m_sigSrc, m_circle.GetPos(), fDsBorder);
                }
            );
        } 
    );
}

float Signal::GetSignalValue() // const
{
    recalcSrcList();

    float fResult { 0.0f };
    for ( auto const& it : m_sigSrc )
        fResult += it.src.GetVoltage().GetValue() * it.fFactor;
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

void Signal::CheckSignal() 
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
