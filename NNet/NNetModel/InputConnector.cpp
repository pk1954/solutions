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
//    m_list = move(src);
}

InputConnector::InputConnector(vector<IoNeuron *> && src)
    :	IoConnector(NobType::Value::inputConnector)
{
    for (auto it : src)
        m_list.push_back(it);
    src.clear();
    //    m_list = move(src);
}

void InputConnector::DrawExterior(DrawContext const & context, tHighlight const type) const
{
    if (Size() > 1)
    {
        MicroMeterPnt umPnt1     { m_list.front()->GetPos() }; 
        MicroMeterPnt umPnt2     { m_list.back ()->GetPos() }; 
        MicroMeterPnt umPntDir   { umPnt2 - umPnt1 };
        MicroMeterPnt umPntOff   { umPntDir.ScaledTo(NEURON_RADIUS * 1.2f) };
        MicroMeterPnt umOrthoVec { umPntDir.OrthoVector().ScaledTo(NEURON_RADIUS * 0.3f) };
        umPnt1 -= umOrthoVec;
        umPnt2 -= umOrthoVec;
        context.DrawLine
        (
            umPnt1 - umPntOff, 
            umPnt2 + umPntOff, 
            m_list.front()->GetExtension() * 1.2f, 
            GetExteriorColor(type)
       );
    }
    IoConnector::DrawExterior(context, type);
}

