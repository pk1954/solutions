// InputConnector.cpp 
//
// NNetModel

#include "stdafx.h"
#include "DrawContext.h"
#include "IoNeuron.h"
#include "InputConnector.h"

InputConnector::InputConnector()
    :	IoConnector(NobType::Value::inputConnector)
{}

InputConnector::InputConnector(vector<IoNeuron *> & src)
    :	IoConnector(NobType::Value::inputConnector)
{
    for (auto it : src)
        m_list.push_back(it);
    src.clear();
}

InputConnector::InputConnector(vector<IoNeuron *> && src)
    :	IoConnector(NobType::Value::inputConnector)
{
    for (auto it : src)
        m_list.push_back(it);
    src.clear();
}

void InputConnector::DrawExterior(DrawContext const & context, tHighlight const type) const
{
    if (Size() > 1)
    {
        MicroMeterPnt umPnt1     { m_list.front()->GetPos() }; 
        MicroMeterPnt umPnt2     { m_list.back ()->GetPos() }; 
        MicroMeterPnt umPntDir   { umPnt2 - umPnt1 };
        MicroMeterPnt umPntOff   { umPntDir.ScaledTo(NEURON_RADIUS * 1.2f) };
        MicroMeterPnt umOrthoVec { umPntDir.OrthoVector().ScaledTo(NEURON_RADIUS * 0.7f) };
        umPnt1 -= umOrthoVec;
        umPnt2 -= umOrthoVec;
        context.DrawLine
        (
            umPnt1 - umPntOff        + umOrthoVec * 2.0f,
            umPnt1 - umPntOff * 0.8f + umOrthoVec * 2.0f, 
            m_list.front()->GetExtension() * 2.0f, 
            GetExteriorColor(type)
        );
        context.DrawLine
        (
            umPnt2 + umPntOff        + umOrthoVec * 2.0f,
            umPnt2 + umPntOff * 0.8f + umOrthoVec * 2.0f,
            m_list.front()->GetExtension() * 2.0f, 
            GetExteriorColor(type)
        );
        context.DrawLine
        (
            umPnt1 - umPntOff + umOrthoVec * 0.4f, 
            umPnt2 + umPntOff + umOrthoVec * 0.4f, 
            m_list.front()->GetExtension() * 0.25f, 
            GetExteriorColor(type)
        );
    }
    IoConnector::DrawExterior(context, type);
}

