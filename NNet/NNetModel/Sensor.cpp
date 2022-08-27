// Sensor.cpp : 
//
// NNetModel

#pragma once

#include <iostream> 
#include "Signal.h"
#include "UPNobList.h"
#include "Sensor.h"

import BasicTypes;
import DrawContext;
import NNetColors;

using std::endl;
using std::wcout;

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

void Sensor::Recalc(UPNobList const & list) 
{
    m_dataPoints.clear();
    list.Apply2AllC<Pipe>([this](Pipe const & pipe) { add2list(pipe); });
    NotifyAll(false);
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

void Sensor::add2list(Pipe const & pipe) 
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

Sensor::SigDataPoint const * Sensor::findDataPoint(MicroMeterPnt const & umPnt) const
{
    for (auto const & it : m_dataPoints)
        if (it.dataPointCircle().Includes(umPnt))
            return &it;
    return nullptr;
}

void Sensor::DrawDataPoints(DrawContext const & context) const
{
    for (auto const& it : m_dataPoints)
    {
        mV     const voltage { it.m_pPipe->GetVoltage(it.m_segNr) };
        ColorF const col     { it.m_pPipe->GetInteriorColor(voltage) };
        context.FillCircle(it.dataPointCircle(), col);
    }
}

void Sensor::WriteInfo(wostream& out) const
{
    out << Signal::SIGSRC_CIRCLE; 
    out << GetCircle();
    out << endl;
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
