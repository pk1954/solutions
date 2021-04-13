// Connector.cpp 
//
// NNetModel

#include "stdafx.h"
#include "CalcOrthoVector.h"
#include "ConnectionNeuron.h"
#include "Connector.h"

Connector::Connector()
  :	Compound( ShapeType::Value::connector )
{};

void Connector::apply2All(function<void(ConnNeuron &)> const & func)
{
    Compound::Apply2All([&](Shape & s){ func(static_cast<ConnNeuron &>(s)); } );
}

void Connector::apply2All(function<void(ConnNeuron const &)> const & func) const
{
    Compound::Apply2All([&](Shape const & s){ func(static_cast<ConnNeuron const &>(s)); } );
}

void Connector::AlignDirection()
{
    ShapePtrList<BaseKnot> list;
    Compound::Apply2All([&](Shape & s) { list.Add(static_cast<BaseKnot *>(&s)); } );
    MicroMeterLine  const umLine(GetFirst().GetPosition(), GetLast().GetPosition());
    MicroMeterPoint const umPntDir { CalcOrthoVector(umLine, list) };
    apply2All([&](ConnNeuron & c){ c.SetDirVector(umPntDir); } );
}

MicroMeterPoint const Connector::GetPosition() const 
{ 
    return (GetFirst().GetPosition() + GetLast().GetPosition()) / 2.0f; 
}

void Connector::Prepare()
{
    apply2All([&](ConnNeuron & n){ n.Prepare(); } );
}

bool const Connector::CompStep()
{
    bool bStop { false };
    apply2All([&](ConnNeuron & n){ if (n.CompStep()) bStop = true; } );
    return bStop;
}

void Connector::Recalc()
{
    apply2All([&](ConnNeuron & n){ n.Recalc(); } );
}

void Connector::Apply2All(function<void(ConnNeuron const &)> const & func) const
{
    apply2All([&](ConnNeuron const & n){ func(n); } );
}                        

void Connector::MoveShape(MicroMeterPoint const & delta)       
{
    apply2All([&](Shape & s){ s.MoveShape(delta); } );
}

void Connector::RotateShape(MicroMeterPoint const & umPntPivot, Radian const radDelta)
{
    apply2All([&](ConnNeuron & n){ n.RotateShape(umPntPivot, radDelta); } );
}

void Connector::Rotate(MicroMeterPoint const & umPntOld, MicroMeterPoint const & umPntNew)
{
    MicroMeterPoint const umPntPivot { GetPosition() };
    Radian          const radOld     { Vector2Radian(umPntOld - umPntPivot) };
    Radian          const radNew     { Vector2Radian(umPntNew - umPntPivot) };
    Radian          const radDelta   { radNew - radOld };
    RotateShape(umPntPivot, radDelta);
}                        

bool const Connector::IsIncludedIn(MicroMeterRect const & umRect) const 
{
    bool bRes { false };
    apply2All([&](ConnNeuron const & n){ if (n.IsIncludedIn(umRect)) bRes = true; } );
    return bRes;
}

bool const Connector::Includes(MicroMeterPoint const & umPnt) const
{
    bool bRes { false };
    apply2All([&](ConnNeuron const & n) { if (n.Includes(umPnt)) bRes = true; } );
    return bRes;
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
