// Connector.cpp 
//
// NNetModel

#include "stdafx.h"
#include "MicroMeterPosDir.h"
#include "IoNeuronList.h"
#include "IoNeuron.h"
#include "Neuron.h"
#include "Connector.h"

Connector::Connector(NobIoMode const ioMode)
  :	Nob(NobType::Value::connector),
    m_IoMode(ioMode)
{
    m_upList = make_unique<IoNeuronList>();
}

Connector::Connector(unique_ptr<IoNeuronList> upSrc)
  :	Nob(NobType::Value::connector),
    m_IoMode(upSrc->GetFirst().GetIoMode())
{
    m_upList = move(upSrc);
}

Connector::Connector(Connector const & src)   // copy constructor
  :	Nob(src),
    m_IoMode(src.GetIoMode())
{
    m_upList = make_unique<IoNeuronList>(*src.m_upList.get());
}

bool const Connector::IsInputConnector () const 
{ 
    return m_upList->GetFirst().IsInputNob (); 
}

bool const Connector::IsOutputConnector() const 
{ 
    return m_upList->GetFirst().IsOutputNob(); 
}

void Connector::Check() const
{
    Nob::Check();
    m_upList->Check();
}

void Connector::Dump() const
{
    Nob::Dump();
    m_upList->Dump();
}

NobIoMode const Connector::GetIoMode() const 
{ 
    return m_IoMode;
}

void Connector::Select(bool const bOn) 
{ 
    Nob::Select(bOn);
    m_upList->SelectAll(bOn);
}

void Connector::Push(IoNeuron * const p) 
{ 
    m_upList->Add(p); 
}

IoNeuron * const Connector::Pop() 
{ 
    IoNeuron * pRet { & m_upList->GetLast() };
    m_upList->RemoveLast();
    return pRet;
}

size_t const Connector::Size() const 
{ 
    return m_upList->Size(); 
}

IoNeuron const & Connector::GetElem(size_t const nr) const 
{ 
    return m_upList->GetElem(nr); 
}

void Connector::Link(Nob const & nobSrc, Nob2NobFunc const & dstFromSrc)
{
    m_upList->Link(dstFromSrc);
}

void Connector::Clear( )
{
    Nob::Clear();
    m_upList->Clear();
}

void Connector::AlignDirection()
{
    m_upList->AlignDirection();
}

MicroMeterPnt const Connector::GetPos() const 
{ 
    return m_upList->IsEmpty() ? MicroMeterPnt::NULL_VAL() : m_upList->GetPos(); 
}

Radian const Connector::GetDir() const 
{ 
    return m_upList->IsEmpty() ? Radian::NULL_VAL() : m_upList->GetFirst().GetDir();
}

MicroMeterPosDir const Connector::GetPosDir() const 
{ 
    return m_upList->IsEmpty() ? MicroMeterPosDir::NULL_VAL() : MicroMeterPosDir( GetPos(), GetDir() );
}

void Connector::SetParentPointers()
{
    m_upList->SetParentPointers(this);
}

void Connector::ClearParentPointers()
{
    m_upList->ClearParentPointers();
}

void Connector::Prepare()
{
    m_upList->Prepare();
}

bool const Connector::CompStep()
{
    return m_upList->CompStep();
}

void Connector::Recalc()
{
    m_upList->Recalc();
}

void Connector::Apply2All(function<void(IoNeuron const &)> const & func) const
{
    m_upList->Apply2All([&](IoNeuron const & n){ func(n); } );
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
    m_upList->MoveNob(delta);
}

void Connector::RotateNob(MicroMeterPnt const & umPntPivot, Radian const radDelta)
{
    m_upList->RotateNob(umPntPivot, radDelta);
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
    return m_upList->IsIncludedIn(umRect);
}

bool const Connector::Includes(MicroMeterPnt const & umPnt) const
{
    return m_upList->Includes(umPnt);
}

void Connector::DrawExterior(DrawContext const & context, tHighlight const type) const
{
    m_upList->DrawExterior(context, type);
}

void Connector::DrawInterior(DrawContext const & context, tHighlight const type) const
{
    m_upList->DrawInterior(context, type);
}

void Connector::Expand(MicroMeterRect & umRect) const
{
    m_upList->Expand(umRect);
}

Connector const * Cast2Connector(Nob const * pNob)
{
    assert( pNob->IsConnector() );
    return static_cast<Connector const *>(pNob);
}

Connector * Cast2Connector(Nob * pNob)
{
    assert( pNob->IsConnector() );
    return static_cast<Connector *>(pNob);
}

wostream & operator<< (wostream & out, Connector const & v)
{
    out << * v.m_upList.get();
    return out;
}
