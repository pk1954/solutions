// Vector2D.ixx
//
// ToolBox\Utilities

module;

#include <memory>
#include <vector>

export module Vector2D;

import Raster;

using std::vector;
using std::unique_ptr;
using std::make_unique;

export template <typename UNIT>
class Vector2D
{
public:
    using UnitVector = vector<UNIT>;

    Vector2D(RasterPoint const& size)
    {
        m_rows.reserve(size.m_y);
        for (int i = 0; i < size.m_y; ++i)
            m_rows.push_back(make_unique<UnitVector>(size.m_x, UNIT::NULL_VAL()));
    }

    UnitVector& GetLine(RasterIndex const ry)
    {
        return *m_rows.at(ry).get();
    }

    UnitVector const& GetLine(RasterIndex const ry) const
    {
        return *m_rows.at(ry).get();
    }

    void Set(RasterPoint const& rp, UNIT val)
    {
        GetLine(rp.m_y).at(rp.m_x) = val;
    }

    void Set(UNIT const newVal)
    {
        Apply2AllPixels([newVal](UNIT& val) { val = newVal; });
    }

    UNIT Get(RasterPoint const& rp) const
    {
        return GetLine(rp.m_y).at(rp.m_x);
    }

    UNIT Get(UnitVector const &line, RasterIndex const rx) const
    {
        return line.at(rx);
    }

    Vector2D& operator*= (float const factor)
    {
        Apply2AllPixels([factor](UNIT &val) { val *= factor; });
        return *this;
    }

    void Apply2AllPixels(auto const& func)
    {
        for (auto &upLine : m_rows)
            for (UNIT& val : *upLine.get())
                func(val);
    }

    void Apply2AllPixelsC(auto const& func) const
    {
        for (auto const& upLine : m_rows)
            for (UNIT& val : *upLine.get())
                func(val);
    }

    UNIT GetMax() const
    {
        UNIT valMax { 0.0f };
        Apply2AllPixelsC([&valMax](UNIT const v){ if (v > valMax) valMax = v; });
        return valMax;
    }

private:
    vector<unique_ptr<UnitVector>> m_rows;
};
