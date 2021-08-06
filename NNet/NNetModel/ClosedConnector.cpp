// ClosedConnector.cpp
//
// NNetModel

#include "stdafx.h"
#include "DrawContext.h"
#include "Neuron.h"
#include "IoNeuron.h"
#include "ClosedConnector.h"

void ClosedConnector::DrawExterior(DrawContext const & context, tHighlight const highLightType) const
{
    if (m_list.size() > 1)
    {
        context.DrawLine
        (
            m_list.front()->GetPos(), 
            m_list.back ()->GetPos(), 
            m_list.front()->GetExtension() * 2.0f, 
            GetExteriorColor(highLightType)
       );
    }

    IoConnector::DrawExterior(context, highLightType);
}

ClosedConnector & Cast2ClosedConnector(Nob & nob)
{
    assert(nob.IsClosedConnector());
    return static_cast<ClosedConnector &>(nob);
}
