// Connector.cpp 
//
// NNetModel

#include "stdafx.h"
#include "IoNeuron.h"
#include "Connector.h"

Connector::Connector()
  :	Nob(NobType::Value::connector)
{};

Connector::Connector(IoNeuronList const & src)
  :	Nob(NobType::Value::connector)
{
    m_list = src;
}

void Connector::Check() const
{
    Nob::Check();
    m_list.Check();
}

void Connector::Dump() const
{
    Nob::Dump();
    m_list.Dump();
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
    m_list.Link(dstFromSrc);
}

void Connector::Clear( )
{
    Nob::Clear();
    m_list.Clear();
}

void Connector::AlignDirection()
{
    m_list.AlignDirection();
}

MicroMeterPnt const Connector::GetPos() const 
{ 
    return m_list.GetPos(); 
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
    m_list.SetParentPointers(this);
}

void Connector::ClearParentPointers()
{
    m_list.ClearParentPointers();
}

void Connector::Prepare()
{
    m_list.Prepare();
}

bool const Connector::CompStep()
{
    return m_list.CompStep();
}

void Connector::Recalc()
{
    m_list.Recalc();
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
    m_list.MoveNob(delta);
}

void Connector::RotateNob(MicroMeterPnt const & umPntPivot, Radian const radDelta)
{
    m_list.RotateNob(umPntPivot, radDelta);
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
    return m_list.IsIncludedIn(umRect);
}

bool const Connector::Includes(MicroMeterPnt const & umPnt) const
{
    return m_list.Includes(umPnt);
}

void Connector::DrawExterior(DrawContext const & context, tHighlight const type) const
{
    m_list.DrawExterior(context, type);
}

void Connector::DrawInterior(DrawContext const & context, tHighlight const type) const
{
    m_list.DrawInterior(context, type);
}

void Connector::Expand(MicroMeterRect & umRect) const
{
    m_list.Expand(umRect);
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
