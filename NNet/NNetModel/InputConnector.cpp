// InputConnector.cpp 
//
// NNetModel

module;

#include <vector>   
#include <memory>   
#include "Resource.h"

module NNetModel:InputConnector;

import Types;
import DrawContext;
import :tHighlight;
import :SignalGenerator;
import :NNetParameters;
import :NobType;
import :InputConnector;
import :InputLine;
import :IoConnector;
import :IoLine;

using std::make_unique;
using std::vector;

InputConnector::InputConnector(vector<IoLine *> && src)
  : IoConnector(NobType::Value::inputConnector)
{
    m_list = move(src);
}

void InputConnector::DrawExterior(DrawContext const & context, tHighlight const type) const
{
    if (Size() > 1)
    {
        static float const WIDTH  { 0.2f };
        static float const FACTOR { (1.0f + WIDTH) * 0.5f };
        static float const OFFSET { 1.0f + WIDTH * 0.5f };

        MicroMeterPnt       umPnt1         { m_list.front()->GetPos() }; 
        MicroMeterPnt       umPnt2         { m_list.back ()->GetPos() }; 
        MicroMeterPnt const umPntDir       { (umPnt2 - umPnt1) };
        MicroMeterPnt const umPntDirScaled { umPntDir.ScaledTo(NEURON_RADIUS) };
        MicroMeterPnt const umPntOrtho     { - GetDirVector().ScaledTo(NEURON_RADIUS) };
        MicroMeterPnt const umPntDirOff    { umPntDirScaled * OFFSET };
        MicroMeterPnt const umPntOrtOff    { umPntOrtho     * OFFSET };
        MicroMeterPnt const umPntOrtCor    { umPntOrtho     * FACTOR };
        MicroMeter    const umWidth        { NEURON_RADIUS  * WIDTH };
        umPnt1 += umPntOrtCor;
        umPnt2 += umPntOrtCor;
        context.DrawLine
        (
            umPnt1 - umPntDirOff - umPntOrtOff, 
            umPnt1 - umPntDirOff + umPntOrtOff,
            umWidth, 
            GetExteriorColor(type)
        );
        context.DrawLine
        (
            umPnt2 + umPntDirOff - umPntOrtOff, 
            umPnt2 + umPntDirOff + umPntOrtOff,
            umWidth, 
            GetExteriorColor(type)
        );
        context.DrawLine
        (
            umPnt1 - umPntDirScaled - umPntOrtho,
            umPnt2 + umPntDirScaled - umPntOrtho,
            umWidth, 
            GetExteriorColor(type)
        );
    }
    IoConnector::DrawExterior(context, type);
}

InputLine& InputConnector::GetElem(size_t const nr) const
{
    return *Cast2InputLine(&IoConnector::GetElem(nr));
};

SignalGenerator * InputConnector::GetSigGen()
{
    return GetElem(0).GetSigGen();
}

SignalGenerator const * InputConnector::GetSigGenC() const
{
    return GetElem(0).GetSigGenC();
}

void InputConnector::SetSigGen(SignalGenerator* const pSigGen)
{
    Apply2All
    (
        [pSigGen](IoLine & n)
        { 
            Cast2InputLine(&n)->SetSigGen(pSigGen); 
        }
    );
}
