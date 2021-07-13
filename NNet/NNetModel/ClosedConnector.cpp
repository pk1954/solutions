// ClosedConnector.cpp
//
// NNetModel

#include "stdafx.h"
#include "ClosedConnector.h"

ClosedConnector::ClosedConnector
( 
    MicroMeterPnt const & pnt,  // not used 
    IoNeuronList listInput, 
    IoNeuronList listOutput 
)
  :	Nob(NobType::Value::closedConnector)
{
    m_listInput  = move(listInput);
    m_listOutput = move(listOutput);
    Check();
}

ClosedConnector::ClosedConnector
( 
    MicroMeterPnt const & pnt,  // not used 
    Connector & connInput, 
    Connector & connOutput 
)
  :	Nob(NobType::Value::closedConnector)
{
    m_listInput  = move(connInput .GetIoNeurons());
    m_listOutput = move(connOutput.GetIoNeurons());
}

void ClosedConnector::Check() const
{
    m_listInput .Check();
    m_listInput .Apply2All([&](IoNeuron &n){ assert(n.IsInputNeuron()); });
    m_listOutput.Check();
    m_listOutput.Apply2All([&](IoNeuron &n){ assert(n.IsOutputNeuron()); });
}

void ClosedConnector::Dump() const
{
    Nob::Dump();
    wcout << L" in";
    m_listInput .Dump();
    wcout << L" out";
    m_listOutput.Dump();
    wcout << endl;
}

MicroMeterPnt const ClosedConnector::GetPos() const
{
   return (m_listInput.GetPos() + m_listOutput.GetPos()) * 0.5f;
}

void ClosedConnector::DrawExterior(DrawContext const & context, tHighlight const highLightType) const
{
    m_listInput .DrawExterior(context, highLightType);
    m_listOutput.DrawExterior(context, highLightType);
}

void ClosedConnector::DrawInterior(DrawContext const &context, tHighlight const highLightType) const
{
    m_listInput .DrawInterior(context, highLightType);
    m_listOutput.DrawInterior(context, highLightType);
}

void ClosedConnector::Expand(MicroMeterRect & umRect) const
{
    m_listInput .Expand(umRect);
    m_listOutput.Expand(umRect);
}

bool const ClosedConnector::IsIncludedIn(MicroMeterRect const & rect) const
{
    return m_listInput.IsIncludedIn(rect) || m_listOutput.IsIncludedIn(rect);
}

bool const ClosedConnector::Includes(MicroMeterPnt const & umPnt) const
{
    return m_listInput.Includes(umPnt) || m_listOutput.Includes(umPnt);
}

void ClosedConnector::RotateNob(MicroMeterPnt const & umPntPivot, Radian const radian)
{   
    m_listInput .RotateNob(umPntPivot, radian);
    m_listOutput.RotateNob(umPntPivot, radian);
}

void ClosedConnector::Prepare()
{
    m_listInput .Prepare();
    m_listOutput.Prepare();
}

bool const ClosedConnector::CompStep()
{
    bool bStop { false };
    if (m_listInput .CompStep()) bStop = true;
    if (m_listOutput.CompStep()) bStop = true;
    return bStop;
}

void ClosedConnector::Recalc()
{
    m_listInput .Recalc();
    m_listOutput.Recalc();
}

void ClosedConnector::Clear()
{
    m_listInput .Clear();
    m_listOutput.Clear();
}

void ClosedConnector::Link(Nob const & nobSrc, Nob2NobFunc const & dstFromSrc)
{
    m_listInput .Link(dstFromSrc);
    m_listOutput.Link(dstFromSrc);

    //auto const & src = static_cast<ClosedConnector const &>(nobSrc);
    //m_pInputConnector  = static_cast<Connector *>(dstFromSrc(src.m_pInputConnector ));
    //m_pOutputConnector = static_cast<Connector *>(dstFromSrc(src.m_pOutputConnector));
}

void ClosedConnector::MoveNob(MicroMeterPnt const & delta)       
{
    m_listInput .MoveNob(delta);
    m_listOutput.MoveNob(delta);
}

void ClosedConnector::SetParentPointers()
{
    m_listInput .SetParentPointers(this);
    m_listOutput.SetParentPointers(this);
}

void ClosedConnector::ClearParentPointers()
{
    m_listInput .ClearParentPointers();
    m_listOutput.ClearParentPointers();
}

