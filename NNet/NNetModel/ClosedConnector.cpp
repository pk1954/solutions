// ClosedConnector.cpp
//
// NNetWindows

#include "stdafx.h"
#include "ClosedConnector.h"

ClosedConnector::ClosedConnector
( 
    Connector & connInput, 
    Connector & connOutput 
)
  :	Nob(NobType::Value::closedConnector)
{
    m_pInputConnector  = &connInput;
    m_pOutputConnector = &connOutput;
}

void ClosedConnector::Check() const
{
    m_pInputConnector ->Check();
    m_pOutputConnector->Check();
}

void ClosedConnector::Dump() const
{
    m_pInputConnector ->Dump();
    m_pOutputConnector->Dump();
}

MicroMeterPnt const ClosedConnector::GetPos() const
{
   return (m_pInputConnector->GetPos() + m_pOutputConnector->GetPos()) * 0.5f;
}

void ClosedConnector::DrawExterior(DrawContext const & context, tHighlight const highLightType) const
{
    m_pInputConnector ->DrawExterior(context, highLightType);
    m_pOutputConnector->DrawExterior(context, highLightType);
}

void ClosedConnector::DrawInterior(DrawContext const &context, tHighlight const highLightType) const
{
    m_pInputConnector ->DrawInterior(context, highLightType);
    m_pOutputConnector->DrawInterior(context, highLightType);
}

void ClosedConnector::Expand(MicroMeterRect & umRect) const
{
    m_pInputConnector ->Expand(umRect);
    m_pOutputConnector->Expand(umRect);
}

bool const ClosedConnector::IsIncludedIn(MicroMeterRect const & rect) const
{
    bool bRes = m_pInputConnector ->IsIncludedIn(rect) 
        || m_pOutputConnector->IsIncludedIn(rect);
    return bRes;
}

bool const ClosedConnector::Includes(MicroMeterPnt const & umPnt) const
{
    return m_pInputConnector ->Includes(umPnt)
        || m_pOutputConnector->Includes(umPnt);
}

void ClosedConnector::RotateNob(MicroMeterPnt const & umPntPivot, Radian const radian)
{   
    m_pInputConnector ->RotateNob(umPntPivot, radian);
    m_pOutputConnector->RotateNob(umPntPivot, radian);
}

void ClosedConnector::Prepare()
{
    m_pInputConnector ->Prepare();
    m_pOutputConnector->Prepare();
}

bool const ClosedConnector::CompStep()
{
    bool bStop { false };
    if (m_pInputConnector ->CompStep()) bStop = true;
    if (m_pOutputConnector->CompStep()) bStop = true;
    return bStop;
}

void ClosedConnector::Recalc()
{
    m_pInputConnector ->Recalc();
    m_pOutputConnector->Recalc();
}

void ClosedConnector::Clear()
{
    m_pInputConnector ->Clear();
    m_pOutputConnector->Clear();
}

void ClosedConnector::Link(Nob const & nobSrc, Nob2NobFunc const & dstFromSrc)
{
    auto const & src = static_cast<ClosedConnector const &>(nobSrc);
    m_pInputConnector  = static_cast<Connector *>(dstFromSrc(src.m_pInputConnector ));
    m_pOutputConnector = static_cast<Connector *>(dstFromSrc(src.m_pOutputConnector));
}

void ClosedConnector::Select(bool const bOn, bool const bRecursive) 
{ 
    Nob::Select(bOn);
    if ( bRecursive )
    {
        m_pInputConnector ->Select(bOn, false);
        m_pOutputConnector->Select(bOn, false);
    }
}

void ClosedConnector::MoveNob(MicroMeterPnt const & delta)       
{
    m_pInputConnector ->MoveNob(delta);
    m_pOutputConnector->MoveNob(delta);
}

void ClosedConnector::SetParentPointers()
{
    m_pInputConnector ->SetParentNob(this);
    m_pOutputConnector->SetParentNob(this);
}

void ClosedConnector::ClearParentPointers()
{
    m_pInputConnector ->SetParentNob(nullptr);
    m_pOutputConnector->SetParentNob(nullptr);
}

