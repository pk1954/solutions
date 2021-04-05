// Connector.cpp 
//
// NNetModel

#include "stdafx.h"
#include "ConnectionNeuron.h"
#include "Connector.h"

Connector::Connector()
  :	Shape( ShapeType::Value::connector )
{};

void Connector::CheckShape() const
{
    Shape::CheckShape();
}

void Connector::Dump() const
{
    Shape::Dump();
    for (auto ps : m_list) { wcout << ps << endl; }
}

MicroMeterPoint const Connector::GetPosition() const 
{ 
    return (m_list.front()->GetPosition() + m_list.back()->GetPosition()) / 2.0f; 
}

void Connector::DrawExterior(DrawContext const & context, tHighlight const type) const
{
    for (auto ps : m_list) { ps->DrawExterior(context, type); }
}

void Connector::DrawInterior(DrawContext const & context, tHighlight const type) const
{
    for (auto ps : m_list) { ps->DrawInterior(context, type); }
}

void Connector::Prepare()
{
    for (auto ps : m_list) { ps->Prepare(); }
}

bool const Connector::CompStep()
{
    bool bStop { false };
    for (auto ps : m_list) { if (ps->CompStep()) bStop = true; }
    return bStop;
}

void Connector::Recalc()
{
    for (auto ps : m_list) { ps->Recalc(); }
}

void Connector::MoveShape(MicroMeterPoint const & delta)       
{
    for (auto ps : m_list) { ps->MoveShapeFromParent(delta); }
}

bool const Connector::IsInRect(MicroMeterRect const & umRect) const 
{
    bool bRes { false };
    for (auto ps : m_list) { if (ps->IsInRect(umRect)) bRes = true; }
    return bRes;
}

bool const Connector::IsPointInShape(MicroMeterPoint const & umPnt) const
{
    bool bRes { false };
    for (auto ps : m_list) { if (ps->IsPointInShape(umPnt)) bRes = true; }
    return bRes;
}

void Connector::Expand(MicroMeterRect & umRect) const
{
    Apply2All( [&](CNPtr const & p) { umRect.Expand(p->GetPosition()); } );
}

void Connector::Select(bool const bOn, bool const bRecursive) 
{ 
    Shape::Select(bOn, false);
    if (bRecursive)
        Apply2All( [&](CNPtr & p) { p->Select(bOn, false); } );
}

Connector const * Cast2Connector( Shape const * pShape )
{
    assert( pShape->IsConnector() );
    return static_cast<Connector const *>(pShape);
}

Connector * Cast2Connector( Shape * pShape )
{
    assert( pShape->IsConnector() );
    return static_cast<Connector *>(pShape);
}
