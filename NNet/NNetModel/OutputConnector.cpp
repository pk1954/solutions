// OutputConnector.cpp 
//
// NNetModel

#include "stdafx.h"
#include "IoNeuron.h"
#include "DrawContext.h"
#include "OutputConnector.h"

OutputConnector::OutputConnector()
    :	IoConnector(NobType::Value::outputConnector)
{}

OutputConnector::OutputConnector(vector<IoNeuron *> & src)
    :	IoConnector(NobType::Value::outputConnector)
{
    for (auto it : src)
        m_list.push_back(it);
    src.clear();
}

OutputConnector::OutputConnector(vector<IoNeuron *> && src)
    :	IoConnector(NobType::Value::outputConnector)
{
    for (auto it : src)
        m_list.push_back(it);
    src.clear();
}

void OutputConnector::DrawExterior(DrawContext const & context, tHighlight const type) const
{
    if (Size() > 1)
    {
        MicroMeterPnt umPnt1     { m_list.front()->GetPos() }; 
        MicroMeterPnt umPnt2     { m_list.back ()->GetPos() }; 
        MicroMeterPnt umPntDir   { umPnt2 - umPnt1 };
        MicroMeterPnt umPntOff   { umPntDir.ScaledTo(NEURON_RADIUS * 1.2f) };
        MicroMeterPnt umOrthoVec { -umPntDir.OrthoVector().ScaledTo(NEURON_RADIUS * 0.1f) };
        umPnt1 += umOrthoVec;
        umPnt2 += umOrthoVec;
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
