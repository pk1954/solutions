// Sensor.cpp : 
//
// NNetModel

module;

#include <iostream> 
#include <iomanip>

module NNetModel:Sensor;

import Types;
import IoUtil;
import DrawContext;
import Signals;
import :NNetColors;
import :UPNobList;

using std::endl;
using std::wcout;
using std::wostream;
using std::setprecision;

class NNetModelIO;

Sensor::Sensor
(
    MicroMeterCircle const & circle,
    UPNobList        const & list
)
  : m_circle(circle)
{
    SetSensorSize(list, circle.GetRadius());
}

void Sensor::Dump() const
{
    wcout << L"circle: " << m_circle << endl;
}

mV Sensor::GetSignalValue() const
{
    mV mVResult { 0.0_mV };
    for (auto const & it : m_dataPoints)
        mVResult += it.GetSignalValue();
    return mVResult;
}

float Sensor::GetDistFactor(MicroMeterPnt const & umPnt) const
{
    return m_circle.DistFactor(umPnt);
}

void Sensor::Draw
(
    DrawContext const & context,
    bool        const   bHighlight
) const
{
    context.FillGradientCircle
    (
        m_circle, 
        NNetColors::EEG_SENSOR_1, 
        bHighlight ? NNetColors::EEG_SENSOR_HIGH : NNetColors::EEG_SENSOR_2
    );
}

void Sensor::Recalc(UPNobList const & list) 
{
    m_dataPoints.clear();
    list.Apply2AllC<Pipe>([this](Pipe const & pipe) { add2list(pipe); });
    NotifyAll(false);
}

void Sensor::add2list(Pipe const & pipe) 
{  
    MicroMeter const umResolution { m_circle.GetRadius() };

    pipe.Apply2AllSensorPoints
    (
        umResolution,
        [this](Pipe const& pipe, MicroMeterPnt const& umPnt, Pipe::SegNr const segNr)
        { 
            if (float const fFactor = GetDistFactor(umPnt) > 0.0f)
                m_dataPoints.push_back(SigDataPoint(&pipe, segNr, umPnt, fFactor));
        }
    );
} 

void Sensor::DrawDataPoints(DrawContext const & context) const
{
    for (auto const& it : m_dataPoints)
    {
        mV    const voltage { it.m_pPipe->GetVoltage(it.m_segNr) };
        Color const col     { it.m_pPipe->GetInteriorColor(voltage) };
        context.FillCircle(it.dataPointCircle(), col);
    }
}

void Sensor::WriteInfo(wostream& out) const
{
    out << Signal::SIGSRC_CIRCLE
        << OPEN_BRACKET 
        << OPEN_BRACKET
        << setprecision(10)
        << m_circle.GetPos().GetXvalue()
        << SEPARATOR
        << m_circle.GetPos().GetYvalue()
        << CLOSE_BRACKET
        << ID_SEPARATOR
        << m_circle.GetRadius().GetValue()
        << CLOSE_BRACKET
        << endl;
}

void Sensor::SetSensorPos
(
    UPNobList     const & list,
    MicroMeterPnt const & umPos
) 
{ 
    m_circle.SetPos(umPos); 
    Recalc(list);
}

void Sensor::SetSensorSize
(
    UPNobList  const & list,
    MicroMeter const   umSize
) 
{ 
    m_circle.SetRadius(umSize); 
    Recalc(list);
}

void Sensor::MoveSensor
(
    UPNobList     const & list,
    MicroMeterPnt const & umDelta
) 
{ 
    SetSensorPos(list, m_circle.GetPos() + umDelta);
}

void Sensor::SizeSensor
(
    UPNobList const & list,
    float     const   factor
) 
{ 
    SetSensorSize(list, GetRadius() * factor); 
}

void Sensor::RotateSensor
(
    MicroMeterPnt const & umPntPivot, 
    Radian        const   radDelta
)
{
    m_circle.Rotate(umPntPivot, radDelta);
}
