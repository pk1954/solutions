// Connector.cpp 
//
// NNetModel

#include "stdafx.h"
#include "CalcOrthoVector.h"
#include "ConnectionNeuron.h"
#include "Connector.h"

Connector::Connector()
  :	Nob(NobType::Value::connector)
{};

Connector::Connector( NobPtrList<ConnNeuron> const & src )
  :	Nob(NobType::Value::connector)
{
    src.Apply2All([&](ConnNeuron & n) { Push(&n); });
}

void Connector::Check() const
{
    Nob::Check();
}

void Connector::Dump() const
{
    Nob::Dump();
    m_list.Apply2All([&](ConnNeuron const & s){ wcout << s << endl; } );
}

NobIoMode const Connector::GetIoMode() const 
{ 
    return IsInputConnector() ? NobIoMode::input : NobIoMode::output; 
}

ConnNeuron * const Connector::Pop() 
{ 
    ConnNeuron * pRet { & m_list.GetLast() };
    m_list.RemoveLast();
    return pRet;
}

void Connector::Link(Nob const & nobSrc, Nob2NobFunc const & dstFromSrc)
{
    Clear();
    static_cast<Connector const &>(nobSrc).Apply2All
    (
        [&](ConnNeuron const & c) 
        { 
            Push(static_cast<ConnNeuron *>(dstFromSrc(& c))); 
        }
    );
}

void Connector::Clear( )
{
    Nob::Clear();
    m_list.Clear();
}

void Connector::AlignDirection()
{
    MicroMeterLine  const umLine(m_list.GetFirst().GetPos(), m_list.GetLast().GetPos());
    MicroMeterPoint const umPntDir { CalcOrthoVector(umLine, m_list) };
    m_list.Apply2All([&](ConnNeuron & connNeuron){ connNeuron.SetDirVector(umPntDir); } );
}

MicroMeterPoint const Connector::GetPos() const 
{ 
    return (m_list.GetFirst().GetPos() + m_list.GetLast().GetPos()) / 2.0f; 
}

Radian const Connector::GetDir() const 
{ 
    return m_list.GetFirst().GetRad();
}

MicroMeterPosDir const Connector::GetPosDir() const 
{ 
    return MicroMeterPosDir( GetPos(), GetDir() );
}

void Connector::SetPos(MicroMeterPoint const & umPos)
{
    MoveNob(umPos - GetPos());
}

void Connector::SetPosDir(MicroMeterPosDir const & umPosDir)
{
    SetDir(umPosDir.GetDir());
    SetPos(umPosDir.GetPos());
}

void Connector::SetDir(Radian const radianNew)
{
    RotateNob(GetPos(), radianNew - GetDir());
}

void Connector::SetParentPointers()
{
    m_list.Apply2All([&](ConnNeuron & n){ n.SetParentNob(this); } );
}

void Connector::ClearParentPointers()
{
    m_list.Apply2All([&](ConnNeuron & n){ n.SetParentNob(nullptr); } );
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

void Connector::Apply2All(function<void(ConnNeuron const &)> const & func) const
{
    m_list.Apply2All([&](ConnNeuron const & n){ func(n); } );
}                        

void Connector::MoveNob(MicroMeterPoint const & delta)       
{
    m_list.Apply2All([&](ConnNeuron & s){ s.MoveNob(delta); } );
}

void Connector::RotateNob(MicroMeterPoint const & umPntPivot, Radian const radDelta)
{
    m_list.Apply2All([&](ConnNeuron & n){ n.RotateNob(umPntPivot, radDelta); } );
}

void Connector::Rotate(MicroMeterPoint const & umPntOld, MicroMeterPoint const & umPntNew)
{
    MicroMeterPoint const umPntPivot { GetPos() };
    Radian          const radOld     { Vector2Radian(umPntOld - umPntPivot) };
    Radian          const radNew     { Vector2Radian(umPntNew - umPntPivot) };
    Radian          const radDelta   { radNew - radOld };
    RotateNob(umPntPivot, radDelta);
}                        

void Connector::Select(bool const bOn, bool const bRecursive) 
{ 
    Nob::Select(bOn);
    if (bRecursive)
        m_list.Apply2All([&](ConnNeuron & s){ s.Select(bOn, false); } );
}

bool const Connector::IsIncludedIn(MicroMeterRect const & umRect) const 
{
    bool bRes { false };
    m_list.Apply2All([&](ConnNeuron const & n){ if (n.IsIncludedIn(umRect)) bRes = true; } );
    return bRes;
}

bool const Connector::Includes(MicroMeterPoint const & umPnt) const
{
    bool bRes { false };
    m_list.Apply2All([&](ConnNeuron const & n) { if (n.Includes(umPnt)) bRes = true; } );
    return bRes;
}

void Connector::DrawExterior(DrawContext const & context, tHighlight const type) const
{
    m_list.Apply2All([&](ConnNeuron const & s){ s.DrawExterior(context, type); } );
}

void Connector::DrawInterior(DrawContext const & context, tHighlight const type) const
{
    m_list.Apply2All([&](ConnNeuron const & s){ s.DrawInterior(context, type); } );
}

void Connector::Expand(MicroMeterRect & umRect) const
{
    m_list.Apply2All([&](ConnNeuron const & s){ umRect.Expand(s.GetPos()); } );
}

Connector const * Cast2Connector( Nob const * pNob )
{
    assert( pNob->IsConnector() );
    return static_cast<Connector const *>(pNob);
}

Connector * Cast2Connector( Nob * pNob )
{
    assert( pNob->IsConnector() );
    return static_cast<Connector *>(pNob);
}
