// MicroMeterPntVector.cpp
//
// NNetModel

module;

#include <vector>
#include <cassert>
#include <algorithm>
#include <iostream>

module NNetModel:MicroMeterPntVector;

import Types;
import SaveCast;
import IoConstants;
import :IoLine;

using std::max;
using std::vector;
using std::wostream;
using std::ranges::max_element;

MicroMeterPntVector::MicroMeterPntVector(vector<IoLine *> const & nobList)
{
    for (auto pNob : nobList) 
        if (pNob)
            Add(pNob->GetPosDir());
}

unsigned int MicroMeterPntVector::Size() const 
{ 
    return Cast2UnsignedInt(m_list.size()); 
}

MicroMeterPosDir MicroMeterPntVector::GetPosDir(size_t const i) const
{
    assert(i < Size());
    return m_list[i];
}

void MicroMeterPntVector::SetPosDir(unsigned int const ui, MicroMeterPosDir const & posDir)
{
    assert(ui < Size());
    m_list[ui] = posDir;
}

void MicroMeterPntVector::SetDir(Radian const radian)
{
    Apply2All([&](MicroMeterPosDir & umPosDir) { umPosDir.SetDir(radian); });
}

void MicroMeterPntVector::SetDir(MicroMeterPntVector const & rhs)
{
    assert(rhs.Size() == Size());
    for (int i = 0; i < m_list.size(); ++i)
        m_list[i].SetDir(rhs.GetPosDir(i).GetDir());
}

void MicroMeterPntVector::SetPos(MicroMeterPntVector const & rhs)
{
    assert(rhs.Size() == Size());
    for (int i = 0; i < m_list.size(); ++i)
        m_list[i].SetPos(rhs.GetPosDir(i).GetPos());
}

void MicroMeterPntVector::Clear()
{
    m_list.clear();
}

void MicroMeterPntVector::Resize(size_t const size)
{
    m_list.resize(size);
}

void MicroMeterPntVector::Add(MicroMeterPnt const & pos, Radian const dir)
{
    m_list.push_back(MicroMeterPosDir(pos, dir));
}

void MicroMeterPntVector::Add(MicroMeterPosDir const & posDir)
{
    m_list.push_back(posDir);
}

bool MicroMeterPntVector::operator==(MicroMeterPntVector const& rhs) const
{
    assert(m_list.size() == rhs.m_list.size());
    for (int i = 0; i < m_list.size(); ++i)
        if (m_list[i] != rhs.m_list[i])
            return false;
    return true; 
}

MicroMeterPntVector& MicroMeterPntVector::operator+= (MicroMeterPntVector const & rhs) 
{ 
    assert(m_list.size() == rhs.m_list.size());
    for (int i = 0; i < m_list.size(); ++i)
        m_list[i] += rhs.m_list[i];
    return * this; 
}

MicroMeterPntVector& MicroMeterPntVector::operator-= (MicroMeterPntVector const & rhs) 
{ 
    assert(m_list.size() == rhs.m_list.size());
    for (int i = 0; i < m_list.size(); ++i)
        m_list[i] -= rhs.m_list[i];
    return * this; 
}

MicroMeterPntVector& MicroMeterPntVector::operator*= (float const factor) 
{ 
    for (int i = 0; i < m_list.size(); ++i)
        m_list[i] *= factor;
    return * this; 
}

MicroMeterPntVector operator+ (MicroMeterPntVector const & a, MicroMeterPntVector const & b) 
{ 
    MicroMeterPntVector res { a }; 
    res += b; 
    return res; 
};

MicroMeterPntVector operator- (MicroMeterPntVector const & a, MicroMeterPntVector const & b) 
{ 
    MicroMeterPntVector res { a }; 
    res -= b; 
    return res; 
};

MicroMeterPntVector operator* (MicroMeterPntVector const & a, float const f) 
{ 
    MicroMeterPntVector res { a }; 
    res *= f; 
    return res; 
};

wostream & operator<< (wostream & out, MicroMeterPntVector const & v)
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

Radian MicroMeterPntVector::FindMaxRadian() const 
{
    if (m_list.empty())
        return Radian::NULL_VAL();
    MicroMeterPosDir const maxElement = * max_element
    (
        m_list, 
        [](MicroMeterPosDir const & a, MicroMeterPosDir const & b) { return a.GetDir() < b.GetDir(); }
    );
    Radian const radMax { maxElement.GetDir() };
    return radMax;
}

MicroMeter MicroMeterPntVector::FindMaxPos() const
{
    if (m_list.empty())
        return MicroMeter::NULL_VAL();
    MicroMeterPosDir const maxElement = * max_element
    (
        m_list, 
        [](MicroMeterPosDir const & a, MicroMeterPosDir const & b) { return Hypot(a) < Hypot(b); }
    );
    return Hypot(maxElement);
}

MicroMeterLine MicroMeterPntVector::GetLine() const
{
    return MicroMeterLine(m_list.front().GetPos(), m_list.back().GetPos());
}

void MicroMeterPntVector::Align(MicroMeterPnt const& umPntStart, MicroMeterPnt const & umPntOffset)
{	
    MicroMeterPnt umPnt { umPntStart };
    Apply2All
    (	
        [&](MicroMeterPosDir & posDir)	
        { 
            posDir.SetPos(umPnt);
            umPnt += umPntOffset;
        }	
   );
}

void MicroMeterPntVector::Align(MicroMeterLine const & umLine, MicroMeter umDist)
{
    MicroMeterPnt const umVector           { umLine.GetVector() };
    MicroMeter    const umLineLengthTarget { umDist * gapCount() };
    MicroMeterPnt const umPntSingleVector  { umVector.ScaledTo(umDist) };
    MicroMeterPnt const umPntLineTarget    { umVector.ScaledTo(umLineLengthTarget) };
    MicroMeterPnt const umPntTargetStart   { umLine.GetCenter() - umPntLineTarget * 0.5f };
    Align(umPntTargetStart, umPntSingleVector);
}

void MicroMeterPntVector::Align(MicroMeterLine const & umLine)
{
    Align(umLine, umLine.Length() / gapCount());
}

void MicroMeterPntVector::Pack(MicroMeter umDist)
{
    Align(GetLine(), umDist);
}

unsigned int CalcNrOfSteps
(
    MicroMeterPntVector const & umPntVectorStart,
    MicroMeterPntVector const & umPntVectorTarget
)
{
    MicroMeterPntVector const umPntVectorDiff { umPntVectorTarget - umPntVectorStart };
    unsigned int        const uiStepsFromRot  { CalcNrOfSteps(umPntVectorDiff.FindMaxRadian()) };
    unsigned int        const uiStepsFromMove { CalcNrOfSteps(umPntVectorDiff.FindMaxPos()) };
    unsigned int        const uiSteps         { max(uiStepsFromRot, uiStepsFromMove) + 1 };
    return uiSteps;
}
