// InputConnector.cpp 
//
// NNetModel

#include "stdafx.h"
#include "Resource.h"
#include "DrawContext.h"
#include "IoNeuron.h"
#include "InputConnector.h"

InputConnector::InputConnector(vector<IoNeuron *> && src)
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

void InputConnector::AppendMenuItems(AddMenuFunc const & add) const
{
    add(IDM_SIGNAL_DESIGNER);
    add(IDM_TRIGGER_STIMULUS);
    IoConnector::AppendMenuItems(add);
}

mV InputConnector::WaveFunction(fMicroSecs const time) const
{
    mV const mVact { m_signalGenerator.GetVoltage(time) };
    float m_factorW = 1.0f / m_pParameters->PulseWidth().GetValue();
    float m_factorU = 4.0f * m_factorW * mVact.GetValue();
    return mV(m_factorU * time.GetValue() * (1.0f - time.GetValue() * m_factorW));
}
