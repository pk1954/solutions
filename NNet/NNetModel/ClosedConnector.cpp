// ClosedConnector.cpp
//
// NNetWindows

#include "stdafx.h"
#include "ClosedConnector.h"

ClosedConnector::ClosedConnector
( 
    Connector & connA, 
    Connector & connB 
)
  :	Shape(ShapeType::Value::closedConnector)
{
    if ( connA.IsInputConnector() && connB.IsOutputConnector() )
    {
        m_pInputConnector  = &connA;
        m_pOutputConnector = &connB; 
    }
    else if ( connB.IsInputConnector() && connA.IsOutputConnector() )
    {
        m_pInputConnector  = &connB;
        m_pOutputConnector = &connA; 
    }
    else
    {
        assert( false );
    }
}

void ClosedConnector::CheckShape() const
{
    m_pInputConnector ->CheckShape();
    m_pOutputConnector->CheckShape();
}

void ClosedConnector::Dump() const
{
    m_pInputConnector ->Dump();
    m_pOutputConnector->Dump();
}

MicroMeterPoint const ClosedConnector::GetPos() const
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
    return m_pInputConnector ->IsIncludedIn(rect) 
        || m_pOutputConnector->IsIncludedIn(rect);
}

bool const ClosedConnector::Includes(MicroMeterPoint const & umPnt) const
{
    return m_pInputConnector ->Includes(umPnt)
        || m_pOutputConnector->Includes(umPnt);
}

void ClosedConnector::RotateShape(MicroMeterPoint const & umPntPivot, Radian const radian)
{   
    m_pInputConnector ->RotateShape(umPntPivot, radian);
    m_pOutputConnector->RotateShape(umPntPivot, radian);
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

void ClosedConnector::Link(Shape const & shapeSrc, function<Shape * (Shape const *)> const & dstFromSrc)
{
    auto const & src = static_cast<ClosedConnector const &>(shapeSrc);
    m_pInputConnector  = static_cast<Connector *>(dstFromSrc(src.m_pInputConnector ));
    m_pOutputConnector = static_cast<Connector *>(dstFromSrc(src.m_pOutputConnector));
}

void ClosedConnector::Select(bool const bOn, bool const bRecursive) 
{ 
    Shape::Select(bOn);
    if ( bRecursive )
    {
        m_pInputConnector ->Select(bOn, false);
        m_pOutputConnector->Select(bOn, false);
    }
}

void ClosedConnector::MoveShape(MicroMeterPoint const & delta)       
{
    m_pInputConnector ->MoveShape(delta);
    m_pOutputConnector->MoveShape(delta);
}

void ClosedConnector::SetParentPointers()
{
    m_pInputConnector ->SetParent(this);
    m_pOutputConnector->SetParent(this);
}

void ClosedConnector::ClearParentPointers()
{
    m_pInputConnector ->SetParent(nullptr);
    m_pOutputConnector->SetParent(nullptr);
}

