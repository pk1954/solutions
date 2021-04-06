// Connector.cpp 
//
// NNetModel

#include "stdafx.h"
#include "CalcOrthoVector.h"
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
    m_list.Apply2All([&](ConnNeuron & n){ wcout << n << endl; } );
}

void Connector::AlignDirection()
{
    MicroMeterLine  const umLine(m_list.GetFirst().GetPosition(), m_list.GetLast().GetPosition());
    MicroMeterPoint const umPntDir { CalcOrthoVector(umLine, m_list) };
    m_list.Apply2All([&](ConnNeuron & n){ n.SetDirVector(umPntDir); } );
}

MicroMeterPoint const Connector::GetPosition() const 
{ 
    return (m_list.GetFirst().GetPosition() + m_list.GetLast().GetPosition()) / 2.0f; 
}

void Connector::DrawExterior(DrawContext const & context, tHighlight const type) const
{
    m_list.Apply2All([&](ConnNeuron & n){ n.DrawExterior(context, type); } );
}

void Connector::DrawInterior(DrawContext const & context, tHighlight const type) const
{
    m_list.Apply2All([&](ConnNeuron & n){ n.DrawInterior(context, type); } );
}

void Connector::Prepare()
{
    m_list.Apply2All([&](ConnNeuron & n){ n.Prepare(); } );
}

bool const Connector::CompStep()
{
    bool bStop { false };
    m_list.Apply2All([&](ConnNeuron & n){ if (n.CompStep()) bStop = true; } );
    return bStop;
}

void Connector::Recalc()
{
    m_list.Apply2All([&](ConnNeuron & n){ n.Recalc(); } );
}

void Connector::MoveShape(MicroMeterPoint const & delta)       
{
    m_list.Apply2All([&](ConnNeuron & n){ n.MoveShapeFromParent(delta); } );
}

void Connector::SetParentPointers()
{
    m_list.Apply2All([&](ConnNeuron & n){ n.SetParent(this); } );
}

void Connector::ClearParentPointers()
{
    m_list.Apply2All([&](ConnNeuron & n){ n.SetParent(nullptr); } );
}

void Connector::Apply2All(function<void(ConnNeuron const *)> const & func) const
{
    m_list.Apply2All([&](ConnNeuron & n){ func(&n); } );
}                        

bool const Connector::Includes(MicroMeterRect const & umRect) const 
{
    bool bRes { false };
    m_list.Apply2All([&](ConnNeuron & n){ if (n.Includes(umRect)) bRes = true; } );
    return bRes;
}

bool const Connector::Includes(MicroMeterPoint const & umPnt) const
{
    bool bRes { false };
    m_list.Apply2All([&](ConnNeuron & n){ if (n.Includes(umPnt)) bRes = true; } );
    return false;
}

void Connector::Expand(MicroMeterRect & umRect) const
{
    m_list.Apply2All([&](ConnNeuron & n){ umRect.Expand(n.GetPosition()); } );
}

void Connector::Select(bool const bOn, bool const bRecursive) 
{ 
    Shape::Select(bOn, false);
    if (bRecursive)
        m_list.Apply2All([&](ConnNeuron & n){ n.Select(bOn, false); } );
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
