// Signal.cpp 
//
// NNetModel

#include "stdafx.h"
#include "DrawContext.h"
#include "NNetColors.h"
#include "NNetModelReaderInterface.h"
#include "Signal.h"

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
    m_data.clear();
    m_observable.RegisterObserver(this);
}

Signal::~Signal()
{
    m_observable.UnregisterObserver(this);
}

float const Signal::GetSignalValue() const
{
    float fResult   { 0.0f };
    float fDsBorder { m_circle.GetRadius().GetValue() * m_circle.GetRadius().GetValue() };

    m_nmri.GetUPNobs().Apply2All<BaseKnot>
    (		
        [&](BaseKnot const & b) 
        {  
            float fDsBaseKnot { DistSquare(b.GetPos(), m_circle.GetPos()) };
            if (fDsBaseKnot < fDsBorder)  // is b in circle?
            {
                mV    voltage { b.GetVoltage() };
                float fFactor { 1.0f - fDsBaseKnot / fDsBorder };
                fResult += voltage.GetValue() * fFactor;
            }
        } 
   );

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

int const Signal::time2index(fMicroSecs const usParam) const
{
    fMicroSecs const timeTilStart { usParam - m_timeStart };
    float      const fNrOfPoints  { timeTilStart / m_nmri.GetTimeResolution() };
    int              index        { static_cast<int>(roundf(fNrOfPoints)) };
    if (index >= m_data.size())
        index = Cast2UnsignedInt(m_data.size() - 1);
    return index;
}

fMicroSecs const Signal::index2time(int const index) const
{
    float      const fNrOfPoints  { static_cast<float>(index) };
    fMicroSecs const timeTilStart { m_nmri.GetTimeResolution() * fNrOfPoints };
    fMicroSecs const usResult     { timeTilStart + m_timeStart };
    return usResult;
}

float const Signal::GetDataPoint(fMicroSecs const time) const
{
    int index { time2index(time) };
    return (index < 0) ? NAN : m_data[index];
}

fMicroSecs const Signal::FindNextMaximum(fMicroSecs const time) const
{
    int index { time2index(time) };
    if (index < 0)
        return fMicroSecs::NULL_VAL();
    if ((index > 0) && (m_data[index-1] > m_data[index])) // falling values, go left
    {   
        while ((--index > 0) && (m_data[index-1] >= m_data[index]));
    }
    else   // climbing values, go right
    {
        while ((index < m_data.size() - 1) && (m_data[index] <= m_data[index+1]))
            ++index;
    }
    return index2time(index);
}

void Signal::Notify(bool const bImmediate)
{
    m_data.push_back(GetSignalValue());
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
