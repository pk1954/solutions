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
    for (auto & ups : m_list) { wcout << ups << endl; }
}

void Connector::DrawExterior(DrawContext const & context, tHighlightType const type) const
{
    for (auto & ups : m_list) { ups->DrawExterior(context, type); }
}

void Connector::DrawInterior(DrawContext const & context, tHighlightType const type) const
{
    for (auto & ups : m_list) { ups->DrawInterior(context, type); }
}

void Connector::Prepare()
{
    for (auto & ups : m_list) { ups->Prepare(); }
}

bool const Connector::CompStep()
{
    bool bStop { false };
    for (auto & ups : m_list) { if (ups->CompStep()) bStop = true; }
    return bStop;
}

void Connector::Recalc()
{
    for (auto & ups : m_list) { ups->Recalc(); }
}

void Connector::MoveShape(MicroMeterPoint const & delta)       
{
    for (auto & ups : m_list) { ups->MoveShape(delta); }
}

bool const Connector::IsInRect(MicroMeterRect const & umRect) const 
{
    bool bRes { false };
    for (auto & ups : m_list) { if (ups->IsInRect(umRect)) bRes = true; }
    return bRes;
}

bool Connector::IsPointInShape(MicroMeterPoint const & umPnt) const
{
    bool bRes { false };
    for (auto & ups : m_list) { if (ups->IsPointInShape(umPnt)) bRes = true; }
    return bRes;
}

void Connector::Expand( MicroMeterRect & umRect ) const
{
    Apply2All
    (
        [&]( unique_ptr<ConnectionNeuron> const & upConnectionNeuron )
        {
            umRect.Expand( upConnectionNeuron->GetPosition() );
        }
    );
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
