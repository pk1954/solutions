// IoConnector.cpp 
//
// NNetModel

#include "stdafx.h"
#include "MicroMeterPosDir.h"
#include "IoNeuron.h"
#include "Neuron.h"
#include "IoConnector.h"

using std::make_unique;

IoConnector::IoConnector(NobType const nobType)
    :	Nob(nobType)
{
    m_upList = make_unique<IoNeuronList>();
}

IoConnector::IoConnector(IoConnector const & src)
  : Nob(src)
{
    m_upList = make_unique<IoNeuronList>(*src.m_upList.get());
}

void IoConnector::Check() const
{
    Nob::Check();
    m_upList->Check();
}

void IoConnector::Dump() const
{
    Nob::Dump();
    m_upList->Dump();
}

void IoConnector::Select(bool const bOn) 
{ 
    Nob::Select(bOn);
    m_upList->SelectAll(bOn);
}

void IoConnector::Push(IoNeuron * const p) 
{ 
    m_upList->Add(p); 
}

IoNeuron * const IoConnector::Pop() 
{ 
    IoNeuron * pRet { & m_upList->GetLast() };
    m_upList->RemoveLast();
    return pRet;
}

size_t const IoConnector::Size() const 
{ 
    return m_upList->Size(); 
}

IoNeuron const & IoConnector::GetElem(size_t const nr) const 
{ 
    return m_upList->GetElem(nr); 
}

void IoConnector::Link(Nob const & nobSrc, Nob2NobFunc const & dstFromSrc)
{
    m_upList->Link(dstFromSrc);
}

void IoConnector::Reverse()
{
    m_upList->Reverse();
}

void IoConnector::Clear()
{
    Nob::Clear();
    m_upList->Clear();
}

void IoConnector::AlignDirection()
{
    m_upList->AlignDirection();
}

MicroMeterPnt const IoConnector::GetPos() const 
{ 
    return m_upList->IsEmpty() ? MicroMeterPnt::NULL_VAL() : m_upList->GetPos(); 
}

Radian const IoConnector::GetDir() const 
{ 
    return m_upList->GetDir();
}

MicroMeterPosDir const IoConnector::GetPosDir() const 
{ 
    return m_upList->IsEmpty() ? MicroMeterPosDir::NULL_VAL() : MicroMeterPosDir(GetPos(), GetDir());
}

void IoConnector::SetParentPointers()
{
    m_upList->SetParentPointers(this);
}

void IoConnector::ClearParentPointers()
{
    m_upList->ClearParentPointers();
}

void IoConnector::Prepare()
{
    m_upList->Prepare();
}

bool const IoConnector::CompStep()
{
    return m_upList->CompStep();
}

void IoConnector::Recalc()
{
    m_upList->Recalc();
}

void IoConnector::Apply2All(function<void(IoNeuron &)> const & func) const
{
    m_upList->Apply2All([&](IoNeuron & n){ func(n); });
}                        

void IoConnector::SetDir(Radian const radianNew)
{
    m_upList->RotateNobs(GetPos(), radianNew - GetDir());
}

void IoConnector::SetPos(MicroMeterPnt const & umPos)
{
    MoveNob(umPos - GetPos());
}

void IoConnector::SetPosDir(MicroMeterPosDir const & umPosDir)
{
    MicroMeterPnt const pos { GetPos() };
    assert(pos.IsNotNull());
    m_upList->RotateNobs(pos, umPosDir.GetDir() - GetDir());
    m_upList->MoveNobs(umPosDir.GetPos() - pos);
}

void IoConnector::MoveNob(MicroMeterPnt const & delta)       
{
    m_upList->MoveNobs(delta);
}

void IoConnector::RotateNob(MicroMeterPnt const & umPntPivot, Radian const radDelta)
{
    m_upList->RotateNobs(umPntPivot, radDelta);
}

void IoConnector::Rotate(MicroMeterPnt const & umPntOld, MicroMeterPnt const & umPntNew)
{
    MicroMeterPnt const umPntPivot { GetPos() };
    Radian        const radOld     { Vector2Radian(umPntOld - umPntPivot) };
    Radian        const radNew     { Vector2Radian(umPntNew - umPntPivot) };
    Radian        const radDelta   { radNew - radOld };
    RotateNob(umPntPivot, radDelta);
}                        

bool const IoConnector::IsIncludedIn(MicroMeterRect const & umRect) const 
{
    return m_upList->IsIncludedIn(umRect);
}

bool const IoConnector::Includes(MicroMeterPnt const & umPnt) const
{
    return m_upList->Includes(umPnt);
}

void IoConnector::DrawExterior(DrawContext const & context, tHighlight const type) const
{
    m_upList->DrawExterior(context, type);
}

void IoConnector::DrawInterior(DrawContext const & context, tHighlight const type) const
{
    m_upList->DrawInterior(context, type);
}

void IoConnector::Expand(MicroMeterRect & umRect) const
{
    m_upList->Expand(umRect);
}

IoConnector const * Cast2IoConnector(Nob const * pNob)
{
    assert(pNob->IsIoConnector());
    return static_cast<IoConnector const *>(pNob);
}

IoConnector * Cast2IoConnector(Nob * pNob)
{
    assert(pNob->IsIoConnector());
    return static_cast<IoConnector *>(pNob);
}

wostream & operator<< (wostream & out, IoConnector const & v)
{
    out << * v.m_upList.get();
    return out;
}
