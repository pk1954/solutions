// IoConnector.cpp 
//
// NNetModel

#include "stdafx.h"
#include "Resource.h"
#include "MicroMeterPosDir.h"
#include "IoNeuron.h"
#include "Neuron.h"
#include "IoConnector.h"

using std::make_unique;
using std::wcout;
using std::endl;

IoConnector::IoConnector(NobType const nobType)
    :	Nob(nobType)
{ }

void IoConnector::Check() const
{
    Nob::Check();
    if (m_list.empty())
        return;
    for (auto it : m_list) 
    { 
        assert(it);
        assert(it->IsAnyNeuron());
    }; 
    NobType const nobType { m_list.front()->GetNobType() };
    for (auto it : m_list) 
    { 
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

void IoConnector::Push(IoNeuron * const p) 
{ 
    m_list.push_back(p); 
}

IoNeuron * IoConnector::Pop() 
{ 
    IoNeuron * pRet { m_list.back() };
    m_list.pop_back();
    return pRet;
}

size_t IoConnector::Size() const 
{ 
    return m_list.size(); 
}

IoNeuron const & IoConnector::GetElem(size_t const nr) const 
{ 
    return * m_list.at(nr); 
}

void IoConnector::Link(Nob const & nobSrc, Nob2NobFunc const & dstFromSrc)
{
    for (auto & it : m_list) 
        it = static_cast<IoNeuron *>(dstFromSrc(it));
}

void IoConnector::AlignDirection()
{
    MicroMeterLine const umLine   { m_list.front()->GetPos(), m_list.back()->GetPos() };
    MicroMeterPnt  const umPntDir { CalcOrthoVector(umLine) };
    for (auto it : m_list) { it->SetDirVector(umPntDir); }
}

MicroMeterPnt IoConnector::GetPos() const 
{ 
    assert(!m_list.empty());
    return (m_list.front()->GetPos() + m_list.back()->GetPos()) * 0.5f; 
}

Radian IoConnector::GetDir() const 
{ 
    return m_list.empty() 
        ? Radian::NULL_VAL() 
        : m_list.front()->GetDir();
}

void IoConnector::UnlockDirection() const 
{
    for (auto & it: m_list)
        it->UnlockDirection(); 
}

void IoConnector::LockDirection() const 
{ 
    for (auto & it: m_list)
        it->LockDirection(); 
}

MicroMeterPosDir IoConnector::GetPosDir() const 
{ 
    return m_list.empty() ? MicroMeterPosDir::NULL_VAL() : MicroMeterPosDir(GetPos(), GetDir());
}

void IoConnector::SetParentPointers()
{
    for (auto & it: m_list)
        it->SetParentNob(this);
}

void IoConnector::ClearParentPointers() const
{
    for (auto & it: m_list)
        it->SetParentNob(nullptr);
}

void IoConnector::Recalc()
{
    for (auto & it: m_list)
        it->Recalc();
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

bool IoConnector::IsIncludedIn(MicroMeterRect const & umRect) const 
{
    for (auto it : m_list) { if (it->IsIncludedIn(umRect)) return true; }
    return false;
}

bool IoConnector::Includes(MicroMeterPnt const & umPnt) const
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

MicroMeterLine IoConnector::CalcMaxDistLine() const // find two nobs with maximum distance
{
    return ::CalcMaxDistLine<IoNeuron>(m_list);
}

MicroMeterPnt IoConnector::CalcOrthoVector(MicroMeterLine const & line) const 
{
    return ::CalcOrthoVector<IoNeuron>(m_list, line);
}

void IoConnector::AppendMenuItems(AddMenuFunc const & add) const
{
    add(IDD_DISC_IOCONNECTOR);        
    Nob::AppendMenuItems(add);
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
