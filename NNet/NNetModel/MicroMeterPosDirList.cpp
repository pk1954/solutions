// MicroMeterPosDirList.cpp
//
// NNetModel

module;

#include <vector>
#include <cassert>
#include <algorithm>
#include <iostream>

module NNetModel:MicroMeterPosDirList;

import Types;
import SaveCast;
import IoUtil;
import :IoLine;

using std::max;
using std::vector;
using std::wostream;
using std::ranges::max_element;

MicroMeterPosDirList::MicroMeterPosDirList(vector<IoLine *> const & nobList)
{
    for (auto pNob : nobList) 
        if (pNob)
            m_list.push_back(pNob->GetPosDirAccess());
}

unsigned int MicroMeterPosDirList::Size() const 
{ 
    return Cast2UnsignedInt(m_list.size()); 
}

MicroMeterPosDir const& MicroMeterPosDirList::GetPosDir(size_t const i) const
{
    assert(i < Size());
    return *m_list[i];
}

void MicroMeterPosDirList::SetPosDir(unsigned int const ui, MicroMeterPosDir const & posDir)
{
    assert(ui < Size());
    *m_list[ui] = posDir;
}

void MicroMeterPosDirList::SetDir(Radian const radian)
{
    Apply2All([&](MicroMeterPosDir * pUmPosDir) { pUmPosDir->SetDir(radian); });
}

void MicroMeterPosDirList::SetDir(MicroMeterPosDirList const & rhs)
{
    assert(rhs.Size() == Size());
    for (int i = 0; i < m_list.size(); ++i)
        m_list[i]->SetDir(rhs.GetPosDir(i).GetDir());
}

void MicroMeterPosDirList::SetPos(MicroMeterPosDirList const & rhs)
{
    assert(rhs.Size() == Size());
    for (int i = 0; i < m_list.size(); ++i)
        m_list[i]->SetPos(rhs.GetPosDir(i).GetPos());
}

void MicroMeterPosDirList::Clear()
{
    m_list.clear();
}

void MicroMeterPosDirList::Resize(size_t const size)
{
    m_list.resize(size);
}

//void MicroMeterPosDirList::Add(MicroMeterPnt const & pos, Radian const dir)
//{
//    m_list.push_back(MicroMeterPosDir(pos, dir));
//}

void MicroMeterPosDirList::Add(MicroMeterPosDir * const pPosDir)
{
    m_list.push_back(pPosDir);
}

bool MicroMeterPosDirList::operator==(MicroMeterPosDirList const& rhs) const
{
    assert(m_list.size() == rhs.m_list.size());
    for (int i = 0; i < m_list.size(); ++i)
        if (*m_list[i] != *rhs.m_list[i])
            return false;
    return true; 
}

MicroMeterPosDirList& MicroMeterPosDirList::operator+= (MicroMeterPosDirList const & rhs) 
{ 
    assert(m_list.size() == rhs.m_list.size());
    for (int i = 0; i < m_list.size(); ++i)
        *m_list[i] += *rhs.m_list[i];
    return * this; 
}

MicroMeterPosDirList& MicroMeterPosDirList::operator-= (MicroMeterPosDirList const & rhs) 
{ 
    assert(m_list.size() == rhs.m_list.size());
    for (int i = 0; i < m_list.size(); ++i)
        *m_list[i] -= *rhs.m_list[i];
    return * this; 
}

MicroMeterPosDirList& MicroMeterPosDirList::operator*= (float const factor) 
{ 
    for (int i = 0; i < m_list.size(); ++i)
        *m_list[i] *= factor;
    return * this; 
}

MicroMeterPosDirList operator+ (MicroMeterPosDirList const & a, MicroMeterPosDirList const & b) 
{ 
    MicroMeterPosDirList res { a }; 
    res += b; 
    return res; 
};

MicroMeterPosDirList operator- (MicroMeterPosDirList const & a, MicroMeterPosDirList const & b) 
{ 
    MicroMeterPosDirList res { a }; 
    res -= b; 
    return res; 
};

MicroMeterPosDirList operator* (MicroMeterPosDirList const & a, float const f) 
{ 
    MicroMeterPosDirList res { a }; 
    res *= f; 
    return res; 
};

wostream & operator<< (wostream & out, MicroMeterPosDirList const & v)
{
    out << OPEN_BRACKET << v.m_list.size() << L":";
    for (auto & it: v.m_list)
    {
        out << it;
        if (&it == &v.m_list.back())
            break;
        out << ID_SEPARATOR;
    }
    out << CLOSE_BRACKET;
    return out; 
}

Radian MicroMeterPosDirList::FindMaxRadian() const 
{
    if (m_list.empty())
        return Radian::NULL_VAL();
    MicroMeterPosDir * const pMaxElement = *max_element
    (
        m_list, 
        [](MicroMeterPosDir const *a, MicroMeterPosDir const *b) { return a->GetDir() < b->GetDir(); }
    );
    Radian const radMax { pMaxElement->GetDir() };
    return radMax;
}

MicroMeter MicroMeterPosDirList::FindMaxPos() const
{
    if (m_list.empty())
        return MicroMeter::NULL_VAL();
    MicroMeterPosDir * const pMaxElement = *max_element
    (
        m_list, 
        [](MicroMeterPosDir const *a, MicroMeterPosDir const *b) { return Hypot(*a) < Hypot(*b); }
    );
    return Hypot(*pMaxElement);
}

MicroMeterLine MicroMeterPosDirList::GetLine() const
{
    return MicroMeterLine(m_list.front()->GetPos(), m_list.back()->GetPos());
}

void MicroMeterPosDirList::Align(MicroMeterPnt const& umPntStart, MicroMeterPnt const & umPntOffset)
{	
    MicroMeterPnt umPnt { umPntStart };
    Apply2All
    (	
        [&](MicroMeterPosDir * pPosDir)	
        { 
            pPosDir->SetPos(umPnt);
            umPnt += umPntOffset;
        }	
   );
}

void MicroMeterPosDirList::Align(MicroMeterLine const & umLine, MicroMeter umDist)
{
    MicroMeterPnt const umVector           { umLine.GetVector() };
    MicroMeter    const umLineLengthTarget { umDist * gapCount() };
    MicroMeterPnt const umPntSingleVector  { umVector.ScaledTo(umDist) };
    MicroMeterPnt const umPntLineTarget    { umVector.ScaledTo(umLineLengthTarget) };
    MicroMeterPnt const umPntTargetStart   { umLine.GetCenter() - umPntLineTarget * 0.5f };
    Align(umPntTargetStart, umPntSingleVector);
}

void MicroMeterPosDirList::Align(MicroMeterLine const & umLine)
{
    Align(umLine, umLine.Length() / gapCount());
}

void MicroMeterPosDirList::Pack(MicroMeter umDist)
{
    Align(GetLine(), umDist);
}

unsigned int CalcNrOfSteps
(
    MicroMeterPosDirList const & umPntVectorStart,
    MicroMeterPosDirList const & umPntVectorTarget
)
{
    MicroMeterPosDirList const umPntVectorDiff { umPntVectorTarget - umPntVectorStart };
    unsigned int        const uiStepsFromRot  { CalcNrOfSteps(umPntVectorDiff.FindMaxRadian()) };
    unsigned int        const uiStepsFromMove { CalcNrOfSteps(umPntVectorDiff.FindMaxPos()) };
    unsigned int        const uiSteps         { max(uiStepsFromRot, uiStepsFromMove) + 1 };
    return uiSteps;
}
