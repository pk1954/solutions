// ColorLUT.ixx
//
// Toolbox\Utilities

module;

#include <cassert>
#include <array>
#include <vector>

export module ColorLUT;

import Color;

using std::array;
using std::vector;

export class ColorLUT
{
public:
    ColorLUT()
    {
        m_basePoints.push_back(BasePoint(  0, Color(0.0f, 0.0f, 0.0f, 1.0f)));
        m_basePoints.push_back(BasePoint(255, Color(1.0f, 1.0f, 1.0f, 1.0f)));
        AddBasePoint( 10, Color(D2D1::ColorF::Blue));
        AddBasePoint(255, Color(D2D1::ColorF::Red));
        construct();
    }

    void AddBasePoint
    (
        int   pos,
        Color col
    )
    {
        assert(m_basePoints.size() >= 2);

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

    Color Get(int const pos) const
    {
        return m_table.at(pos);
    }

private:

    void construct()
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
    }

    struct BasePoint
    {
        int   pos;
        Color col;
    };
    vector<BasePoint> m_basePoints;
    array<Color, 256> m_table;
};
