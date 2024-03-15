// ColorLUT.cpp
//
// Toolbox\Utilities

module;

#include <cassert>
#include <array>
#include <vector>
#include <algorithm>

module ColorLUT;

import SaveCast;

using std::min;
using std::ranges::remove_if;

void ColorLUT::AddBasePoint
(
    ColIndex colIndex,
    Color    col
)
{
    for (auto it = m_basePoints.begin(); it < m_basePoints.end(); ++it)
    {
        if (it->colIndex == colIndex)
        {
            it->col = col;
            return;
        }
        if (it->colIndex > colIndex)
        {
            m_basePoints.insert(it, BasePoint(colIndex, col));
            return;
        }
    }

    m_basePoints.push_back(BasePoint(colIndex, col));
}

void ColorLUT::AddBasePoint(ColIndex colIndex)
{
    AddBasePoint(colIndex, Get(colIndex));
}

void ColorLUT::Construct()
{
    Apply2AllRanges
    (
        [this](BasePoint const& bpStart, BasePoint const& bpEnd)
        {
            Color const colDiff { bpEnd.col - bpStart.col };
            int   const posDiff { bpEnd.colIndex - bpStart.colIndex };
            Color const colStep { colDiff / Cast2Float(posDiff) };
            for (int i = bpStart.colIndex; i <= bpEnd.colIndex; ++i)
            {
                m_table[i] = bpStart.col + colStep * Cast2Float(i - bpStart.colIndex);
            }
        }
    );
    NotifyAll(true);
}

Color ColorLUT::Get(ColIndex const colIndex) const
{
    return m_table.at(colIndex);
}

void ColorLUT::RemoveBasePoint(BasePoint* const pBp)
{
    assert(pBp);
    auto offset = pBp - m_basePoints.data();
    assert(offset > 0);
    assert(offset < m_basePoints.size()-1);
    m_basePoints.erase(m_basePoints.begin() + offset);
}

void ColorLUT::Clear()
{
    m_basePoints.clear();
}
