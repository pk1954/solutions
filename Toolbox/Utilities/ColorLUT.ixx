// ColorLUT.ixx
//
// Toolbox\Utilities

export module ColorLUT;

export import Color;

import std;
import Debug;
import Observable;

using std::array;
using std::vector;
using std::uint8_t;
using std::numeric_limits;

export using ColIndex    = uint8_t;
export using BasePointNr = size_t;

export BasePointNr NoBasePoint { numeric_limits<size_t>::max() };

export bool IsBasePointDefined  (BasePointNr const nr) { return nr != NoBasePoint; }
export bool IsBasePointUndefined(BasePointNr const nr) { return nr == NoBasePoint; }

export class ColorLUT : public Observable
{
public:

    ~ColorLUT() = default;

    inline static int MAX_INDEX { 255 };

    static bool IsBorderIndex(ColIndex const i) { return (i == 0) || (i == ColorLUT::MAX_INDEX); }

    void SetColIndex(BasePointNr const, ColIndex const);
    void SetColor   (BasePointNr const, Color const);

    size_t Size() const { return m_basePoints.size(); }

    bool IsMoveable  (BasePointNr const) const;
    bool IsDeleteable(BasePointNr const) const;

    ColIndex GetColIndex(BasePointNr const) const;

    BasePointNr AddBasePoint(ColIndex);
    BasePointNr AddBasePoint(ColIndex, Color);
    void        RemoveBasePoint(BasePointNr const);
    void        Construct();
    Color       GetColor(ColIndex const) const;
    Color       GetColor(BasePointNr const) const;
    void        Clear();

    void Apply2AllRanges(auto const &func)
    {
        Assert(m_basePoints.size() >= 2);
        for (int bpIndex = 0; bpIndex < m_basePoints.size() - 1; ++bpIndex)
            func(bpIndex, bpIndex+1);
    }

    void Apply2AllBasePoints(auto const &func)
    {
        for (int bpIndex = 0; bpIndex < m_basePoints.size(); ++bpIndex)
            func(bpIndex);
    }

private:

    struct BasePoint
    {
        ColIndex colIndex;
        Color    col;
    };

    vector<BasePoint> m_basePoints;
    array<Color, 256> m_table;
};
