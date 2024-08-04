// IoConnector.cpp 
//
// NNetModel

module;

#include <cassert>
#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>
#include "Resource.h"

module NNetModel:IoConnector;

import Types;
import IoUtil;
import DrawContext;
import :tHighlight;
import :NobType;
import :IoLine;
import :Pipe;
import :Nob;

using std::vector;
using std::ranges::for_each;
using std::make_unique;
using std::wostream;
using std::wcout;
using std::endl;

void IoConnector::Check() const
{
    Nob::Check();
    if (m_list.empty())
        return;
    for (auto it : m_list) 
    { 
        assert(it);
        assert(it->IsIoLine());
    } 
    NobType const nobType { m_list.front()->GetNobType() };
    for (auto it : m_list) 
    { 
        assert(it->GetNobType() == nobType);
        it->Check();
    } 
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

void IoConnector::SelectAllConnected(bool const bFirst)
{
    if (!IsSelected() || bFirst)
    {
        Nob::Select(true);
        for_each
        (
            m_list,
            [](IoLine* p) { if (p) p->SelectAllConnected(false); }
        );
    }
}

void IoConnector::Push(IoLine* const p) 
{ 
    p->Connect2IoConnector(this);
    m_list.push_back(p);
}

IoLine * IoConnector::Pop() 
{ 
    IoLine * pRet { m_list.back() };
    m_list.pop_back();
    return pRet;
}

void IoConnector::Link(Nob const & nobSrc, Nob2NobFunc const & dstFromSrc)
{
    for (auto & it : m_list) 
        it = static_cast<IoLine *>(dstFromSrc(it));
    ConnectIoLines();
}

Radian IoConnector::GetDir() const 
{ 
    if (m_radDirection.IsNull() && !m_list.empty())
        m_radDirection = Vector2Radian(::CalcOrthoVector(m_list));
    return m_radDirection;
}

MicroMeterPnt IoConnector::GetPos() const 
{ 
    assert(!m_list.empty());
    return (m_list.front()->GetPos() + m_list.back()->GetPos()) * 0.5f; 
}

void IoConnector::ConnectIoLines()
{
    for (auto & it: m_list)
        it->Connect2IoConnector(this);
    DirectionDirty();
}

void IoConnector::DisconnectIoLines() const
{
    for (auto & it: m_list)
        it->DisconnectFromIoConnector();
}

void IoConnector::PosChanged()
{
    for (auto & it: m_list)
        it->PosChanged();
}

void IoConnector::SetPosNoFix(MicroMeterPnt const& umPos)
{
    MoveNob(umPos - GetPos());
}

void IoConnector::Recalc()
{
    DirectionDirty();
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
    DirectionDirty();
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
    return ::CalcMaxDistLine<IoLine>(m_list);
}

void IoConnector::AppendMenuItems(AddMenuFunc const & add) const
{
    add(IDD_DISC_IOCONNECTOR);        
    Nob::AppendMenuItems(add);
}


void IoConnector::AlignPositions(MicroMeterLine const& umLine)
{
    MicroMeter    const umDist             { NEURON_RADIUS * 2.0f };
    MicroMeterPnt const umVector           { umLine.GetVector() };
    float         const fGapCount          { Cast2Float(Size() - 1) };
    MicroMeter    const umLineLengthTarget { umDist * fGapCount };
    MicroMeterPnt const umPntSingleVector  { umVector.ScaledTo(umDist) };
    MicroMeterPnt const umPntLineTarget    { umVector.ScaledTo(umLineLengthTarget) };
    MicroMeterPnt       umPnt              { umLine.GetCenter() - umPntLineTarget * 0.5f };
    Apply2All
    (
        [&umPnt, &umPntSingleVector](IoLine& ioLine)
        {
            ioLine.SetPos(umPnt);
            umPnt += umPntSingleVector;
        }
    );
}

void IoConnector::SetPositions(vector<MicroMeterPnt> const& posList)
{
    assert(posList.size() == Size());
    for (int i = 0; i < Size(); ++i)
    {
        m_list[i]->SetPos(posList[i]);
    }
}

vector<MicroMeterPnt> IoConnector::GetPositions() const
{
    vector<MicroMeterPnt> result;
    for (auto &elem : m_list)
        result.push_back(elem->GetPos());
    return result;
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

wostream & operator<< (wostream & out, IoConnector const & conn)
{
    out << CURLY_OPEN_BRACKET << conn.Size() << NR_SEPARATOR;
    for (auto & it: conn.m_list)
    {
        out << it->GetId().GetValue();
        if (&it == &conn.m_list.back())
            break;
        out << ID_SEPARATOR;
    }
    out << CURLY_CLOSE_BRACKET;
    return out;
}

MicroMeterPnt CalcOrthoVector(vector<IoLine*> const& list)
{
    int iReverse { 0 };
    MicroMeterLine const umLine { list.front()->GetPos(), list.back()->GetPos() };
    if (list.front()->IsOutputLine())
        for (IoLine* pIoLine : list)
        {
            MicroMeterPnt pnt { pIoLine->GetPipe()->GetStartPoint() };
            if (PointToLine(umLine, pnt) < 0.0_MicroMeter)
                ++iReverse;
            else
                --iReverse;
        }
    else
        for (IoLine* pIoLine : list)
        {
            MicroMeterPnt pnt { pIoLine->GetPipe()->GetEndPoint() };
            if (PointToLine(umLine, pnt) < 0.0_MicroMeter)
                --iReverse;
            else
                ++iReverse;
        }

    MicroMeterPnt orthoVector { umLine.OrthoVector() };
    if (iReverse > 0)
        orthoVector = -orthoVector;
    return orthoVector;
}
