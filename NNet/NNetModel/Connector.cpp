// Connector.cpp 
//
// NNetModel

#include "stdafx.h"
#include "CalcOrthoVector.h"
#include "ConnectionNeuron.h"
#include "Connector.h"

Connector::Connector()
  :	Shape(ShapeType::Value::connector)
{};

void Connector::CheckShape() const
{
    Shape::CheckShape();
}

void Connector::Dump() const
{
    Shape::Dump();
    m_list.Apply2All([&](Shape const & s){ wcout << s << endl; } );
}

void Connector::Link(Shape const & shapeSrc, function<Shape * (Shape const *)> const & dstFromSrc)
{
    Clear();
    static_cast<Connector const &>(shapeSrc).Apply2All([&](Shape const & c) { Push(dstFromSrc(& c)); });
}

void Connector::Clear( )
{
    Shape::Clear();
    m_list.Clear();
}

void Connector::AlignDirection()
{
    MicroMeterLine  const umLine(m_list.GetFirst().GetPos(), m_list.GetLast().GetPos());
    MicroMeterPoint const umPntDir { CalcOrthoVector(umLine, m_list) };
    m_list.Apply2All([&](Shape & c){ static_cast<ConnNeuron &>(c).SetDirVector(umPntDir); } );
}

MicroMeterPoint const Connector::GetPos() const 
{ 
    return (m_list.GetFirst().GetPos() + m_list.GetLast().GetPos()) / 2.0f; 
}

void Connector::SetParentPointers()
{
    m_list.Apply2All([&](Shape & n){ n.SetParent(this); } );
}

void Connector::ClearParentPointers()
{
    m_list.Apply2All([&](Shape & n){ n.SetParent(nullptr); } );
}

void Connector::Prepare()
{
    m_list.Apply2All([&](Shape & n){ n.Prepare(); } );
}

bool const Connector::CompStep()
{
    bool bStop { false };
    m_list.Apply2All([&](Shape & n){ if (n.CompStep()) bStop = true; } );
    return bStop;
}

void Connector::Recalc()
{
    m_list.Apply2All([&](Shape & n){ n.Recalc(); } );
}

void Connector::Apply2All(function<void(Shape const &)> const & func) const
{
    m_list.Apply2All([&](Shape const & n){ func(n); } );
}                        

void Connector::MoveShape(MicroMeterPoint const & delta)       
{
    m_list.Apply2All([&](Shape & s){ s.MoveShape(delta); } );
}

void Connector::RotateShape(MicroMeterPoint const & umPntPivot, Radian const radDelta)
{
    m_list.Apply2All([&](Shape & n){ n.RotateShape(umPntPivot, radDelta); } );
}

void Connector::Rotate(MicroMeterPoint const & umPntPivot, Radian const radDelta)
{
    m_list.Apply2All([&](Shape & s){ RotateShape(umPntPivot, radDelta); } );
}                        

void Connector::Rotate(MicroMeterPoint const & umPntOld, MicroMeterPoint const & umPntNew)
{
    MicroMeterPoint const umPntPivot { GetPos() };
    Radian          const radOld     { Vector2Radian(umPntOld - umPntPivot) };
    Radian          const radNew     { Vector2Radian(umPntNew - umPntPivot) };
    Radian          const radDelta   { radNew - radOld };
    RotateShape(umPntPivot, radDelta);
}                        

void Connector::Select(bool const bOn, bool const bRecursive) 
{ 
    Shape::Select(bOn);
    if (bRecursive)
        m_list.Apply2All([&](Shape & s){ s.Select(bOn, false); } );
}

bool const Connector::IsIncludedIn(MicroMeterRect const & umRect) const 
{
    bool bRes { false };
    m_list.Apply2All([&](Shape const & n){ if (n.IsIncludedIn(umRect)) bRes = true; } );
    return bRes;
}

bool const Connector::Includes(MicroMeterPoint const & umPnt) const
{
    bool bRes { false };
    m_list.Apply2All([&](Shape const & n) { if (n.Includes(umPnt)) bRes = true; } );
    return bRes;
}

void Connector::DrawExterior(DrawContext const & context, tHighlight const type) const
{
    m_list.Apply2All([&](Shape const & s){ s.DrawExterior(context, type); } );
}

void Connector::DrawInterior(DrawContext const & context, tHighlight const type) const
{
    m_list.Apply2All([&](Shape const & s){ s.DrawInterior(context, type); } );
}

void Connector::Expand(MicroMeterRect & umRect) const
{
    m_list.Apply2All([&](Shape const & s){ umRect.Expand(s.GetPos()); } );
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
