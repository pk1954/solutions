// ColorLUT.cpp
//
// Toolbox\Utilities

module ColorLUT;

import std;
import Debug;
import SaveCast;

using std::min;
using std::ranges::remove_if;
using std::distance;

void ColorLUT::SetColIndex
(
    BasePointNr const bpNr,
    ColIndex   const colIndex
)
{
    m_basePoints.at(bpNr).colIndex = colIndex;
}

void ColorLUT::SetColor
(
    BasePointNr const bpNr,
    Color       const col
)
{
    m_basePoints.at(bpNr).col = col;
}

bool ColorLUT::IsMoveable(BasePointNr const bpNr) const
{
    if (IsBasePointUndefined(bpNr))
        return false;
    if ((bpNr == 0) || (bpNr == Size()-1))
        return false;
    return true;
}

bool ColorLUT::IsDeleteable(BasePointNr const bpNr) const
{
    return IsMoveable(bpNr);
}

BasePointNr ColorLUT::AddBasePoint
(
    ColIndex colIndex,
    Color    col
)
{
    for (auto it = m_basePoints.begin(); it < m_basePoints.end(); ++it)
    {
        if (it->colIndex > colIndex)
        {
            BasePointNr bpRes { BasePointNr(distance(m_basePoints.begin(), it)) };
            m_basePoints.insert(it, BasePoint(colIndex, col));
            return bpRes;
        }
    }

    m_basePoints.push_back(BasePoint(colIndex, col));
    return BasePointNr(Size()-1);
}

BasePointNr ColorLUT::AddBasePoint(ColIndex colIndex)
{
    return AddBasePoint(colIndex, GetColor(colIndex));
}

ColIndex ColorLUT::GetColIndex(BasePointNr const bpNr) const
{
    return m_basePoints.at(bpNr).colIndex;
}

Color ColorLUT::GetColor(BasePointNr const bpNr) const
{
    return m_basePoints.at(bpNr).col;
}

void ColorLUT::Construct()
{
    Apply2AllRanges
    (
        [this](BasePointNr const bp0, BasePointNr const bp1)
        {
            BasePoint const& bpStart { m_basePoints.at(bp0) };
            BasePoint const& bpEnd   { m_basePoints.at(bp1) };
            Color     const  colDiff { bpEnd.col - bpStart.col };
            int       const  posDiff { bpEnd.colIndex - bpStart.colIndex };
            Color     const  colStep { colDiff / Cast2Float(posDiff) };
            for (int i = bpStart.colIndex; i <= bpEnd.colIndex; ++i)
            {
                m_table[i] = bpStart.col + colStep * Cast2Float(i - bpStart.colIndex);
            }
        }
    );
    NotifyAll(true);
}

Color ColorLUT::GetColor(ColIndex const colIndex) const
{
    return m_table.at(colIndex);
}

void ColorLUT::RemoveBasePoint(BasePointNr const bpNr)
{
    Assert(bpNr > 0);
    Assert(bpNr < Size()-1);
    m_basePoints.erase(m_basePoints.begin() + bpNr);
}

void ColorLUT::Clear()
{
    m_basePoints.clear();
}
