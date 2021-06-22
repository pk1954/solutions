// Connector.cpp 
//
// NNetModel

#include "stdafx.h"
#include "CalcOrthoVector.h"
#include "IoNeuron.h"
#include "Connector.h"

Connector::Connector()
  :	Nob(NobType::Value::connector)
{};

Connector::Connector( NobPtrList<IoNeuron> const & src )
  :	Nob(NobType::Value::connector)
{
    src.Apply2All([&](IoNeuron & n) { Push(&n); });
}

void Connector::Check() const
{
    Nob::Check();
    m_list.Apply2All([&](IoNeuron const & n){ n.Check(); } );
}

void Connector::Dump() const
{
    Nob::Dump();
    wcout << endl; 
    m_list.Apply2All([&](IoNeuron const & n) { wcout << L"       " << Scanner::COMMENT_SYMBOL << n << endl; });
}

NobIoMode const Connector::GetIoMode() const 
{ 
    return IsInputConnector() ? NobIoMode::input : NobIoMode::output; 
}

IoNeuron * const Connector::Pop() 
{ 
    IoNeuron * pRet { & m_list.GetLast() };
    m_list.RemoveLast();
    return pRet;
}

void Connector::Link(Nob const & nobSrc, Nob2NobFunc const & dstFromSrc)
{
    Clear();
    static_cast<Connector const &>(nobSrc).Apply2All
    (
        [&](IoNeuron const & c) 
        { 
            Push(static_cast<IoNeuron *>(dstFromSrc(& c))); 
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
    MicroMeterLine const umLine(m_list.GetFirst().GetPos(), m_list.GetLast().GetPos());
    MicroMeterPnt  const umPntDir { CalcOrthoVector(umLine, m_list) };
    m_list.Apply2All([&](IoNeuron & ioNeuron){ ioNeuron.SetDirVector(umPntDir); } );
}

MicroMeterPnt const Connector::GetPos() const 
{ 
    return (m_list.GetFirst().GetPos() + m_list.GetLast().GetPos()) / 2.0f; 
}

Radian const Connector::GetDir() const 
{ 
    return m_list.GetFirst().GetDir();
}

MicroMeterPosDir const Connector::GetPosDir() const 
{ 
    return MicroMeterPosDir( GetPos(), GetDir() );
}

void Connector::SetParentPointers()
{
    m_list.Apply2All([&](IoNeuron & n){ n.SetParentNob(this); } );
}

void Connector::ClearParentPointers()
{
    m_list.Apply2All([&](IoNeuron & n){ n.SetParentNob(nullptr); } );
}

void Connector::Prepare()
{
    m_list.Apply2All([&](IoNeuron & n){ n.Prepare(); } );
}

bool const Connector::CompStep()
{
    bool bStop { false };
    m_list.Apply2All([&](IoNeuron & n){ if (n.CompStep()) bStop = true; } );
    return bStop;
}

void Connector::Recalc()
{
    m_list.Apply2All([&](IoNeuron & n){ n.Recalc(); } );
}

void Connector::Apply2All(function<void(IoNeuron const &)> const & func) const
{
    m_list.Apply2All([&](IoNeuron const & n){ func(n); } );
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

void Connector::SetPos(MicroMeterPnt const & umPos)
{
    MoveNob(umPos - GetPos());
}

void Connector::MoveNob(MicroMeterPnt const & delta)       
{
    m_list.Apply2All([&](IoNeuron & s){ s.MoveNob(delta); } );
}

void Connector::RotateNob(MicroMeterPnt const & umPntPivot, Radian const radDelta)
{
    m_list.Apply2All([&](IoNeuron & n){ n.RotateNob(umPntPivot, radDelta); } );
}

void Connector::Rotate(MicroMeterPnt const & umPntOld, MicroMeterPnt const & umPntNew)
{
    MicroMeterPnt const umPntPivot { GetPos() };
    Radian        const radOld     { Vector2Radian(umPntOld - umPntPivot) };
    Radian        const radNew     { Vector2Radian(umPntNew - umPntPivot) };
    Radian        const radDelta   { radNew - radOld };
    RotateNob(umPntPivot, radDelta);
}                        

bool const Connector::IsIncludedIn(MicroMeterRect const & umRect) const 
{
    bool bRes { false };
    m_list.Apply2All([&](IoNeuron const & n){ if (n.IsIncludedIn(umRect)) bRes = true; } );
    return bRes;
}

bool const Connector::Includes(MicroMeterPnt const & umPnt) const
{
    bool bRes { false };
    m_list.Apply2All([&](IoNeuron const & n) { if (n.Includes(umPnt)) bRes = true; } );
    return bRes;
}

void Connector::DrawExterior(DrawContext const & context, tHighlight const type) const
{
    m_list.Apply2All([&](IoNeuron const & s){ s.DrawExterior(context, type); } );
}

void Connector::DrawInterior(DrawContext const & context, tHighlight const type) const
{
    m_list.Apply2All([&](IoNeuron const & s){ s.DrawInterior(context, type); } );
}

void Connector::Expand(MicroMeterRect & umRect) const
{
    m_list.Apply2All([&](IoNeuron const & s){ umRect.Expand(s.GetPos()); } );
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
