// MicroSensor.cpp : 
//
// NNetModel

module;

#include <iostream> 

module NNetModel:MicroSensor;

import Types;
import DrawContext;
import :NNetColors;

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
    out << "NobId";
    out << m_pNob->GetId();
    out << endl;
}

mV MicroSensor::GetSignalValue() const
{
    mV mVResult { m_pNob->GetNextOutput() };
    return mVResult;
}

void MicroSensor::Draw
(
    DrawContext const& context,
    bool        const  bHighlight
) const
{
    MicroMeter       const umWidth  { context.GetCoordC().Transform2logUnit( 5._fPixel) };
    MicroMeter       const umRadius { context.GetCoordC().Transform2logUnit(30._fPixel) };
    MicroMeterCircle const umCircle { m_pNob->GetPos(), umRadius };
    D2D1::ColorF     const col      { bHighlight ? NNetColors::MICRO_SENSOR_HIGH : NNetColors::MICRO_SENSOR };
    context.DrawCircle
    (
        umCircle, 
        col, 
        umWidth
    );
    context.DrawLine
    (
        MicroMeterPnt(m_pNob->GetPos().GetX() - umRadius, m_pNob->GetPos().GetY()),
        MicroMeterPnt(m_pNob->GetPos().GetX() + umRadius, m_pNob->GetPos().GetY()),
        umWidth,
        col
    );
    context.DrawLine
    (
        MicroMeterPnt(m_pNob->GetPos().GetX(), m_pNob->GetPos().GetY() - umRadius),
        MicroMeterPnt(m_pNob->GetPos().GetX(), m_pNob->GetPos().GetY() + umRadius),
        umWidth,
        col
    );
}
