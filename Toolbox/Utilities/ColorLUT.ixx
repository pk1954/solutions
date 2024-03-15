// ColorLUT.ixx
//
// Toolbox\Utilities

module;

#include <cassert>
#include <array>
#include <vector>

export module ColorLUT;

import Color;
import Observable;

using std::array;
using std::vector;
using std::uint8_t;

export using ColIndex = uint8_t;
export class ColorLUT : public Observable
{
public:

    inline static int MAX_INDEX { 255 };

    struct BasePoint
    {
        ColIndex colIndex;
        Color    col;
    };

    size_t Size() const { return m_basePoints.size(); }

    bool IsMoveable(BasePoint const * const pbp)
    {
        if (pbp == &m_basePoints.front())
            return false;
        if (pbp == &m_basePoints.back())
            return false;
        return true;
    }

    void  AddBasePoint(ColIndex);
    void  AddBasePoint(ColIndex, Color);
    void  RemoveBasePoint(BasePoint * const);
    void  Construct();
    Color Get(ColIndex const) const;
    void  Clear();

    void Apply2AllBasePoints(auto const &func)
    {
        for (auto &bp : m_basePoints)
            func(bp);
    }

    void Apply2AllBasePointsC(auto const &func) const
    {
        for (auto const &bp : m_basePoints)
            func(bp);
    }

    void Apply2AllRanges(auto const &func)
    {
        assert(m_basePoints.size() >= 2);
        for (int bpIndex = 0; bpIndex < m_basePoints.size() - 1; ++bpIndex)
            func(m_basePoints[bpIndex], m_basePoints[bpIndex+1] );
    }

private:

    vector<BasePoint> m_basePoints;
    array<Color, 256> m_table;
};
