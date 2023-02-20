// MicroSensor.cpp : 
//
// NNetModel

module;

#include <iostream> 

module NNetModel:MicroSensor;

import Types;
import DrawContext;
import :NNetColors;
import :Signal;

using std::endl;
using std::wcout;
using std::wostream;

MicroSensor::MicroSensor(Nob * const pNob)
    : m_pNob(pNob)
{}

void MicroSensor::Dump() const
{
    wcout << L"NobId: " << m_pNob->GetId() << endl;
}

void MicroSensor::WriteInfo(wostream& out) const
{
    if (!m_bConnected)
        return;

    out << Signal::SIGSRC_NOB;
    out << m_pNob->GetId();
    out << endl;
}

mV MicroSensor::GetSignalValue() const
{
    mV mVResult { m_bConnected ? m_pNob->GetPotential() : 0.0_mV };
    return mVResult;
}

void MicroSensor::Draw
(
    DrawContext const& context,
    bool        const  bHighlight
) const
{
    if (!m_bConnected)
        return;

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
