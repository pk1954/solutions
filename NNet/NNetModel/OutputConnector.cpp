// OutputConnector.cpp 
//
// NNetModel

#include "stdafx.h"
#include "DrawContext.h"
#include "OutputConnector.h"

OutputConnector::OutputConnector()
    :	IoConnector(NobType::Value::outputConnector)
{}

OutputConnector::OutputConnector(unique_ptr<IoNeuronList> upSrc)
    :	IoConnector(NobType::Value::outputConnector)
{
    m_upList = move(upSrc);
}

OutputConnector::OutputConnector(OutputConnector const & src)   // copy constructor
    :	IoConnector(src)
{
    m_upList = make_unique<IoNeuronList>(*src.m_upList.get());
}

void OutputConnector::DrawExterior(DrawContext const & context, tHighlight const type) const
{
    if (Size() > 1)
    {
        MicroMeterPnt umPnt1     { m_upList->GetFirst().GetPos() }; 
        MicroMeterPnt umPnt2     { m_upList->GetLast ().GetPos() }; 
        MicroMeterPnt umPntDir   { umPnt2 - umPnt1 };
        MicroMeterPnt umPntOff   { umPntDir.ScaledTo(NEURON_RADIUS * 1.2f) };
        MicroMeterPnt umOrthoVec { -umPntDir.OrthoVector().ScaledTo(NEURON_RADIUS * 0.2f) };
        umPnt1 -= umOrthoVec;
        umPnt2 -= umOrthoVec;
        context.DrawLine
        (
            umPnt1 - umPntOff, 
            umPnt2 + umPntOff, 
            m_upList->GetFirst().GetExtension() * 1.0f, 
            GetExteriorColor(type)
       );
    }
    IoConnector::DrawExterior(context, type);
}
