// MicroSensor.cpp : 
//
// NNetModel

module NNetModel:MicroSensor;

import std;
import Debug;
import Types;
import DrawContext;
import Signals;
import :NNetColors;
import :Nob;

using std::endl;
using std::wcout;
using std::wostream;

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
    Assert(m_pNob);
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
    Color            const col      { bHighlight ? NNetColors::MICRO_SENSOR_HIGH : NNetColors::MICRO_SENSOR };
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
