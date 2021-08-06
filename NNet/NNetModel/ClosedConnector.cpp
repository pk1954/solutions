// ClosedConnector.cpp
//
// NNetModel

#include "stdafx.h"
#include "DrawContext.h"
#include "Neuron.h"
#include "IoNeuron.h"
#include "ClosedConnector.h"

void ClosedConnector::Check() const
{
    for (auto & it: m_list)
    {
        assert(it->IsNeuron());
        it->Check();
    }
}

void ClosedConnector::Dump() const
{
    Nob::Dump();
    for (auto & it: m_list)
        it->Dump();
    wcout << endl;
}

Neuron * const ClosedConnector::Pop() 
{ 
    Neuron * pRet { m_list.back() };
    m_list.pop_back();
    return pRet;
}

void ClosedConnector::Apply2All(function<void(Neuron const &)> const & func) const
{
    for (auto it: m_list)
        func(*it);
}                        

Radian const ClosedConnector::GetDir() const 
{ 
    return m_list.empty() 
        ? Radian::NULL_VAL() 
        : m_list[0]->GetDir(); 
};

MicroMeterPnt const ClosedConnector::GetPos() const
{
    assert(!m_list.empty());
    return (m_list.front()->GetPos() + m_list.back()->GetPos()) * 0.5f; 
}

void ClosedConnector::DrawExterior(DrawContext const & context, tHighlight const highLightType) const
{
    if (m_list.size() > 1)
    {
        context.DrawLine
        (
            m_list.front()->GetPos(), 
            m_list.back ()->GetPos(), 
            m_list.front()->GetExtension() * 2.0f, 
            GetExteriorColor(highLightType)
       );
    }

    for (auto & it: m_list)
        it->DrawExterior(context, highLightType);
}

void ClosedConnector::DrawInterior(DrawContext const &context, tHighlight const highLightType) const
{
    for (auto & it: m_list)
        it->DrawInterior(context, highLightType);
}

void ClosedConnector::Expand(MicroMeterRect & umRect) const
{
    for (auto & it: m_list)
        it->Expand(umRect);
}

bool const ClosedConnector::IsIncludedIn(MicroMeterRect const & umRect) const
{
    for (auto & it : m_list) { if (it->IsIncludedIn(umRect)) return true; }
    return false;
}

bool const ClosedConnector::Includes(MicroMeterPnt const & umPnt) const
{
    for (auto & it : m_list) { if (it->Includes(umPnt)) return true; }
    return false;
}

void ClosedConnector::RotateNob(MicroMeterPnt const & umPntPivot, Radian const radian)
{   
    for (auto & it: m_list)
        it->RotateNob(umPntPivot, radian);
}

void ClosedConnector::Prepare()
{
    for (auto & it: m_list)
        it->Prepare();
}

bool const ClosedConnector::CompStep()
{
    bool bStop { false };
    for (auto & it: m_list)
        if (it->CompStep()) return true;
    return false;
}

void ClosedConnector::Recalc()
{
    for (auto & it: m_list)
        it->Recalc();
}

void ClosedConnector::Clear()
{
    m_list.clear();
}

void ClosedConnector::Link(Nob const & nobSrc, Nob2NobFunc const & dstFromSrc)
{
    for (auto & it : m_list) 
        it = static_cast<IoNeuron *>(dstFromSrc(it));
}

void ClosedConnector::Select(bool const bOn) 
{ 
    Nob::Select(bOn);
    for (auto & it: m_list)
        it->Select(bOn);
}

void ClosedConnector::MoveNob(MicroMeterPnt const & delta)       
{
    for (auto & it: m_list)
        it->MoveNob(delta);
}

void ClosedConnector::SetParentPointers()
{
    for (auto & it: m_list)
        it->SetParentNob(this);
}

void ClosedConnector::ClearParentPointers()
{
    for (auto & it: m_list)
        it->ClearParentPointers();
}

ClosedConnector const * Cast2ClosedConnector(Nob const * pNob)
{
    assert(pNob->IsClosedConnector());
    return static_cast<ClosedConnector const *>(pNob);
}

ClosedConnector * Cast2ClosedConnector(Nob * pNob)
{
    assert(pNob->IsClosedConnector());
    return static_cast<ClosedConnector *>(pNob);
}

wostream & operator<< (wostream & out, ClosedConnector const & v)
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
