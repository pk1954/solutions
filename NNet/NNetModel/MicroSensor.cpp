// MicroSensor.cpp : 
//
// NNetModel

module;

#include <iostream> 

module NNetModel:MicroSensor;

import Types;
import DrawContext;
import :NNetModelWriterInterface;
import :NNetColors;

using std::endl;
using std::wcout;
using std::wostream;

MicroSensor::MicroSensor
(
    NobId               const nobId,
    NNetModelWriterInterface& nmwi
)
    : m_pNob(nmwi.GetNob(nobId))
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
    mV mVResult { 0.0_mV };
    return mVResult;
}

void MicroSensor::Draw
(
    DrawContext const& context,
    bool        const  bHighlight
) const
{
}
