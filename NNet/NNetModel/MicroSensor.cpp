// MicroSensor.cpp : 
//
// NNetModel

module;

#include <cassert> 
#include <iostream> 

module NNetModel:MicroSensor;

import Types;
import DrawContext;
import :NNetColors;
import :Signal;
import :Nob;

using std::endl;
using std::wcout;
using std::wostream;

MicroSensor::MicroSensor(Nob * const pNob)
    : m_pNob(pNob)
{}

MicroMeterPnt MicroSensor::GetPosition() const
{ 
    return m_pNob->GetCenter(); 
}

NobId MicroSensor::GetNobId() const 
{ 
    return m_pNob->GetId(); 
}

void MicroSensor::Dump() const
{
    wcout << L"NobId: " << m_pNob->GetId() << endl;
}

mV MicroSensor::GetSignalValue() const
{
    assert(m_pNob);
    return m_pNob->GetPotential();
}

void MicroSensor::Draw
(
    DrawContext const& context,
    bool        const  bHighlight
) const
{
    static fPixel const WIDTH { 5._fPixel };

    MicroMeter       const umRadius { context.GetCoordC().Transform2logUnit(30._fPixel) };
    MicroMeterCircle const umCircle { GetPosition(), umRadius };
    D2D1::ColorF     const col      { bHighlight ? NNetColors::MICRO_SENSOR_HIGH : NNetColors::MICRO_SENSOR };
    context.DrawCircle
    (
        umCircle, 
        col, 
        WIDTH
    );
    context.DrawLine
    (
        MicroMeterPnt(GetPosition().GetX() - umRadius, GetPosition().GetY()),
        MicroMeterPnt(GetPosition().GetX() + umRadius, GetPosition().GetY()),
        0._MicroMeter,
        col,
        WIDTH
    );
    context.DrawLine
    (
        MicroMeterPnt(GetPosition().GetX(), GetPosition().GetY() - umRadius),
        MicroMeterPnt(GetPosition().GetX(), GetPosition().GetY() + umRadius),
        0._MicroMeter,
        col,
        WIDTH
    );
}
