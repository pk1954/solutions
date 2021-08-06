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
{ }

IoConnector::IoConnector(IoConnector const & src)
  : Nob(src)
{
    m_list = src.m_list;
}

void IoConnector::Check() const
{
    Nob::Check();
    if (m_list.empty())
        return;
    NobType const nobType { m_list.front()->GetNobType() };
    for (auto it : m_list) 
    { 
        assert(it);
        assert(it->IsAnyNeuron());
        assert(it->GetNobType() == nobType);
        it->Check();
    }; 
}

void IoConnector::Dump() const
{
    Nob::Dump();
    wcout << * this << endl;
}

void IoConnector::Select(bool const bOn) 
{ 
    Nob::Select(bOn);
    for (auto it: m_list)
        it->Select(bOn);
}

void IoConnector::Push(Neuron * const p) 
{ 
    m_list.push_back(p); 
}

Neuron * const IoConnector::Pop() 
{ 
    Neuron * pRet { m_list.back() };
    m_list.pop_back();
    return pRet;
}

size_t const IoConnector::Size() const 
{ 
    return m_list.size(); 
}

Neuron const & IoConnector::GetElem(size_t const nr) const 
{ 
    return * m_list.at(nr); 
}

void IoConnector::Link(Nob const & nobSrc, Nob2NobFunc const & dstFromSrc)
{
    for (auto & it : m_list) 
        it = static_cast<Neuron *>(dstFromSrc(it));
}

void IoConnector::Clear()
{
    Nob::Clear();
    m_list.clear();
}

void IoConnector::AlignDirection()
{
    MicroMeterLine const umLine   { m_list.front()->GetPos(), m_list.back()->GetPos() };
    MicroMeterPnt  const umPntDir { CalcOrthoVector(umLine) };
    for (auto it : m_list) { it->SetDirVector(umPntDir); }
}

MicroMeterPnt const IoConnector::GetPos() const 
{ 
    assert(!m_list.empty());
    return (m_list.front()->GetPos() + m_list.back()->GetPos()) * 0.5f; 
}

Radian const IoConnector::GetDir() const 
{ 
    return m_list.empty() 
        ? Radian::NULL_VAL() 
        : m_list.front()->GetDir();
}

MicroMeterPosDir const IoConnector::GetPosDir() const 
{ 
    return m_list.empty() ? MicroMeterPosDir::NULL_VAL() : MicroMeterPosDir(GetPos(), GetDir());
}

void IoConnector::SetParentPointers()
{
    for (auto & it: m_list)
        it->SetParentNob(this);
}

void IoConnector::ClearParentPointers()
{
    for (auto & it: m_list)
        it->ClearParentPointers();
}

void IoConnector::Prepare()
{
    for (auto it : m_list) 
        it->Prepare();
}

bool const IoConnector::CompStep()
{
    for (auto it : m_list) 
        if (it->CompStep()) return true;
    return false;
}

void IoConnector::Recalc()
{
    for (auto & it: m_list)
        it->Recalc();
}

void IoConnector::Apply2All(function<void(Neuron &)> const & func) const
{
    for (auto pNob : m_list) 
        if (pNob)
            func(* pNob);
}                        

void IoConnector::SetDir(Radian const radianNew)
{
    MicroMeterPnt umPntPivot { GetPos() };
    Radian        radDelta   { radianNew - GetDir() };
    for (auto it : m_list) 
    { 
        it->RotateNob(umPntPivot, radDelta); 
    }
}

void IoConnector::SetPos(MicroMeterPnt const & umPos)
{
    MoveNob(umPos - GetPos());
}

void IoConnector::SetPosDir(MicroMeterPosDir const & umPosDir)
{
    MicroMeterPnt const pos { GetPos() };
    assert(pos.IsNotNull());
    RotateNob(pos, umPosDir.GetDir() - GetDir());
    MoveNob  (umPosDir.GetPos() - pos);
}

void IoConnector::MoveNob(MicroMeterPnt const & delta)       
{
    for (auto & it: m_list)
        it->MoveNob(delta);
}

void IoConnector::RotateNob(MicroMeterPnt const & umPntPivot, Radian const radDelta)
{
    for (auto it : m_list) 
        it->RotateNob(umPntPivot, radDelta);
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
    for (auto it : m_list) { if (it->IsIncludedIn(umRect)) return true; }
    return false;
}

bool const IoConnector::Includes(MicroMeterPnt const & umPnt) const
{
    for (auto it : m_list) 
        if (it->Includes(umPnt)) 
            return true;
    return false;
}

void IoConnector::DrawExterior(DrawContext const & context, tHighlight const type) const
{
    for (auto it : m_list) 
        it->DrawExterior(context, type);
}

void IoConnector::DrawInterior(DrawContext const & context, tHighlight const type) const
{
    for (auto it : m_list) 
        it->DrawInterior(context, type);
}

void IoConnector::Expand(MicroMeterRect & umRect) const
{
    for (auto & it: m_list)
        it->Expand(umRect);
}

MicroMeterLine const IoConnector::CalcMaxDistLine() // find two nobs with maximum distance
{
    return ::CalcMaxDistLine<Neuron>(m_list);
}

MicroMeterPnt const IoConnector::CalcOrthoVector(MicroMeterLine const & line)
{
    return ::CalcOrthoVector<Neuron>(m_list, line);
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
    out << BaseKnot::OPEN_BRACKET << v.m_list.size() << BaseKnot::NR_SEPARATOR;
    for (auto & it: v.m_list)
    {
        out << it->GetId();
        if (&it == &v.m_list.back())
            break;
        out << BaseKnot::ID_SEPARATOR;
    }
    out << BaseKnot::CLOSE_BRACKET;
    return out;
}
