// ColorLUT.ixx
//
// Toolbox\Utilities

module;

#include <cassert>
#include <array>
#include <vector>

export module ColorLUT;

import Color;

using std::min;
using std::array;
using std::vector;

export class ColorLUT
{
public:

    void AddBasePoint
    (
        int   pos,
        Color col
    )
    {
        for (auto it = m_basePoints.begin(); it < m_basePoints.end(); ++it)
        {
            if (it->pos == pos)
            {
                it->col = col;
                return;
            }
            if (it->pos > pos)
            {
                m_basePoints.insert(it, BasePoint(pos, col));
                return;
            }
        }

        m_basePoints.push_back(BasePoint(pos, col));
    }

    void Construct()
    {
        assert(m_basePoints.size() >= 2);
        for (int bpIndex = 0; bpIndex < m_basePoints.size() - 1; ++bpIndex)
        {
            BasePoint const& bpStart  { m_basePoints[bpIndex] };
            BasePoint const& bpEnd    { m_basePoints[bpIndex + 1] };
            Color     const  colStart { bpStart.col };
            Color     const  colEnd   { bpEnd  .col };
            Color     const  colDiff  { colEnd - colStart };
            Color     const  colStep  { colDiff / static_cast<float>(bpEnd.pos - bpStart.pos) };
            for (int colIndex = bpStart.pos; colIndex <= bpEnd.pos; ++colIndex)
            {
                m_table[colIndex] = colStart + colStep * static_cast<float>(colIndex - bpStart.pos);
            }
        }        
        m_basePoints.clear();

    }

    Color Get(int const pos) const
    {
        int const index { min(pos, 255) };
        return m_table.at(index);
    }

    //void Clear()
    //{
    //    m_basePoints.clear();
    //}

private:

    struct BasePoint
    {
        int   pos;
        Color col;
    };
    vector<BasePoint> m_basePoints;
    array<Color, 256> m_table;
};
