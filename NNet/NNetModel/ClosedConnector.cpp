// ClosedConnector.cpp
//
// NNetModel

#include "stdafx.h"
#include "IoNeuronList.h"
#include "Neuron.h"
#include "IoNeuron.h"
#include "Connector.h"
#include "ClosedConnector.h"

//ClosedConnector::ClosedConnector
//( 
//    MicroMeterPnt const & pnt,  // not used 
//    IoNeuronList  const & listInput, 
//    IoNeuronList  const & listOutput 
//)
//  :	Nob(NobType::Value::closedConnector)
//{
//    size_t nrOfNeurons { listInput.Size() };
//    for (size_t i = 0; i < nrOfNeurons; ++i)
//    {
//        IoNeuron      const & inputNeuron  { listInput .GetElem(i) };
//        IoNeuron      const & outputNeuron { listOutput.GetElem(i) };
//        MicroMeterPnt const   umPos        { inputNeuron.GetPos() };
//        unique_ptr<Neuron>    upNeuron     { make_unique<Neuron>(umPos, inputNeuron, outputNeuron) };
//
//    }
////    m_listInput  = move(listInput);
////    m_listOutput = move(listOutput);
////    Check();
//}
//
//ClosedConnector::ClosedConnector
//( 
//    MicroMeterPnt const & pnt,  // not used 
//    Connector & connInput, 
//    Connector & connOutput 
//)
//  :	Nob(NobType::Value::closedConnector)
//{
//    //m_listInput  = move(connInput .GetIoNeurons());
//    //m_listOutput = move(connOutput.GetIoNeurons());
//}

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
    //for (int i = 0; i < m_list.size(); ++i)
    //    m_list[i] = static_cast<IoNeuron *>(dstFromSrc(m_list[i]));
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
    assert( pNob->IsClosedConnector() );
    return static_cast<ClosedConnector const *>(pNob);
}

ClosedConnector * Cast2ClosedConnector(Nob * pNob)
{
    assert( pNob->IsClosedConnector() );
    return static_cast<ClosedConnector *>(pNob);
}
