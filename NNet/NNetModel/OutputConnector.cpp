// OutputConnector.cpp 
//
// NNetModel

module;

#include <vector>

module NNetModel:OutputConnector;

import DrawContext;
import Types;
import :tHighlight;
import :NobType;
import :IoConnector;

using std::vector;

OutputConnector::OutputConnector()
    :	IoConnector(NobType::Value::outputConnector)
{}

OutputConnector::OutputConnector(vector<IoLine *> & src)
    :	IoConnector(NobType::Value::outputConnector)
{
    for (auto it : src)
        m_list.push_back(it);
    src.clear();
    ConnectIoLines();
}

OutputConnector::OutputConnector(vector<IoLine *> && src)
  :	IoConnector(NobType::Value::outputConnector)
{
    m_list = move(src);
    ConnectIoLines();
}

void OutputConnector::DrawExterior(DrawContext const & context, tHighlight const type) const
{
    if (Size() > 1)
    {
        static float const WIDTH  { 0.2f };
        static float const FACTOR { 1.0f + WIDTH };

        MicroMeterPnt       umPnt1      { m_list.front()->GetPos() };
        MicroMeterPnt       umPnt2      { m_list.back ()->GetPos() }; 
        MicroMeterPnt const umPntDir    { umPnt2 - umPnt1 };
        MicroMeterPnt const umPntOrtho  { GetDirVector().ScaledTo(NEURON_RADIUS) };
        MicroMeterPnt const umPntOff    { umPntDir.ScaledTo(NEURON_RADIUS * FACTOR) };
        MicroMeterPnt const umPntOrtCor { umPntOrtho * 0.2f };
        umPnt1 += umPntOrtCor;
        umPnt2 += umPntOrtCor;
        context.DrawLine
        (
            umPnt1 - umPntOff, 
            umPnt2 + umPntOff, 
            m_list.front()->GetExtension() * FACTOR,
            GetExteriorColor(type)
       );
    }
    IoConnector::DrawExterior(context, type);
}
