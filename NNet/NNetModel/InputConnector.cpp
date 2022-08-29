// InputConnector.cpp 
//
// NNetModel

#include <vector>   
#include "Resource.h"
#include "Nob.h"
#include "InputLine.h"
#include "SignalGenerator.h"
#include "IoConnector.h"
#include "InputConnector.h"

import BasicTypes;
import MoreTypes;
import DrawContext;
import tHighlight;
import NNetParameters;
import IoLine;
import NobType;

using std::make_unique;
using std::vector;

InputConnector::InputConnector
(
    Param & param,
    vector<IoLine *> && src
)
  : IoConnector(NobType::Value::inputConnector)
{
    m_list = move(src);
}

void InputConnector::DrawExterior(DrawContext const & context, tHighlight const type) const
{
    if (Size() > 1)
    {
        static float const WIDTH  { 0.2f };
        static float const OFFSET { 1.0f + WIDTH * 0.5f };

        MicroMeter    const umWidth     { NEURON_RADIUS * WIDTH };
        MicroMeterPnt       umPnt1      { m_list.front()->GetPos() }; 
        MicroMeterPnt       umPnt2      { m_list.back ()->GetPos() }; 
        MicroMeterPnt const umPntDir    { (umPnt2 - umPnt1)     .ScaledTo(NEURON_RADIUS) };
        MicroMeterPnt const umPntOrtho  { umPntDir.OrthoVector().ScaledTo(NEURON_RADIUS) };
        MicroMeterPnt const umPntDirOff { umPntDir   * OFFSET };
        MicroMeterPnt const umPntOrtOff { umPntOrtho * OFFSET };
        MicroMeterPnt const umPntOrtCor { umPntOrtho * (1.0f + WIDTH) * 0.5f };
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
            umPnt1 - umPntDir - umPntOrtho, 
            umPnt2 + umPntDir - umPntOrtho, 
            umWidth, 
            GetExteriorColor(type)
        );
    }
    IoConnector::DrawExterior(context, type);
}

InputLine & InputConnector::GetElem(size_t const nr) const 
{     
    return static_cast<InputLine &>(IoConnector::GetElem(nr));  
};

SignalGenerator * InputConnector::GetSigGen()       
{ 
    return GetElem(0).GetSigGen(); 
}

SignalGenerator const * InputConnector::GetSigGenC() const 
{ 
    return GetElem(0).GetSigGenC(); 
}

void InputConnector::SetSigGen(SignalGenerator * const pSigGen) 
{ 
    Apply2All
    (
        [pSigGen](IoLine & n)
        { 
            static_cast<InputLine &>(n).SetSigGen(pSigGen); 
        }
    );
}
