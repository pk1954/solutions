// Vector2D.ixx
//
// ToolBox\Utilities

module;

#include <cassert>
#include <memory>
#include <vector>

export module Vector2D;

import Raster;

using std::vector;
using std::unique_ptr;
using std::make_unique;

export template <typename UNIT>
struct UnitVector
{
    UnitVector(size_t const siz, UNIT const val)
        : m_vector(siz, val)
    {}

    UnitVector& operator*= (float const factor)
    {
        for (UNIT& val : m_vector)
            val *= factor;
        return *this;
    }

    UnitVector& operator+= (UnitVector const &rhs)
    {
        assert(m_vector.size() == rhs.m_vector.size());
        for (int i = 0; i < m_vector.size(); ++i)
            m_vector[i] += rhs.m_vector[i];
        return *this;
    }

    vector<UNIT> m_vector;
};

export template <typename UNIT>
class Vector2D
{
public:
    using ROW = UnitVector<UNIT>;

    Vector2D(RasterPoint const& size)
    {
        m_rows.reserve(size.m_y);
        for (int i = 0; i < size.m_y; ++i)
            m_rows.push_back(make_unique<ROW>(size.m_x, UNIT::NULL_VAL()));
    }

    Vector2D(RasterPoint const& size, UNIT const initVal)
    {
        m_rows.reserve(size.m_y);
        for (int i = 0; i < size.m_y; ++i)
            m_rows.push_back(make_unique<ROW>(size.m_x, initVal));
    }

    ROW& GetRow(RasterIndex const ry)
    {
        return *m_rows.at(ry).get();
    }

    ROW const& GetRow(RasterIndex const ry) const
    {
        return *m_rows.at(ry).get();
    }

    void Set(RasterPoint const& rp, UNIT val)
    {
        GetRow(rp.m_y).m_vector.at(rp.m_x) = val;
    }

    void Set(UNIT const newVal)
    {
        Apply2AllPixels([newVal](UNIT& val) { val = newVal; });
    }

    UNIT Get(RasterPoint const& rp) const
    {
        return GetRow(rp.m_y).at(rp.m_x);
    }

    UNIT Get(ROW const &row, RasterIndex const rx) const
    {
        return row.at(rx);
    }

    Vector2D& operator*= (float const factor)
    {
        Apply2AllRows([factor](ROW &vec) { vec *= factor; });
        return *this;
    }

    Vector2D& operator+= (Vector2D const &rhs)
    {
        assert(m_rows.size() == rhs.m_rows.size());
        for (int i = 0; i < m_rows.size(); ++i)
            *m_rows[i].get() += *rhs.m_rows[i].get();
        return *this;
    }

    void Apply2AllRows(auto const& func)
    {
        for (auto& upRow : m_rows)
            func(*upRow.get());
    }

    void Apply2AllRowsC(auto const& func) const
    {
        for (auto const& upRow : m_rows)
            func(*upRow.get());
    }

    void Apply2AllPixels(auto const& func)
    {
        for (auto& upRow : m_rows)
            for (UNIT& val : upRow->m_vector)
                func(val);
    }

    void Apply2AllPixelsC(auto const& func) const
    {
        for (auto const& upRow : m_rows)
            for (UNIT& val : upRow->m_vector)
                func(val);
    }

    UNIT GetMax() const
    {
        UNIT valMax { 0.0f };
        Apply2AllPixelsC([&valMax](UNIT const v){ if (v > valMax) valMax = v; });
        return valMax;
    }

    void Normalize()
    {
		UNIT unitMax { GetMax() };
		if (unitMax.IsNotZero())
			*this *= 1.0f / unitMax.GetValue();
    }

private:
    vector<unique_ptr<ROW>> m_rows;
};
