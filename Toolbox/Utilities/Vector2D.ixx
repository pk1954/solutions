// Vector2D.ixx
//
// ToolBox\Utilities

module;

#include <cassert>
#include <memory>
#include <vector>
#include <algorithm>

export module Vector2D;

import SaveCast;
import Raster;

using std::vector;
using std::unique_ptr;
using std::make_unique;
using std::ranges::sort;

template <typename UNIT>
struct UnitVector
{
    explicit UnitVector(size_t const siz)
        : m_vector(siz)
    {}

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

    void init(RasterPoint const& rpSize)
    {
        m_rows.reserve(rpSize.m_y);
        for (int i = 0; i < rpSize.m_y; ++i)
            m_rows.push_back(make_unique<ROW>(rpSize.m_x));
    }

    Vector2D() {}

    Vector2D(RasterPoint const& rpSize)
    {
        m_width = rpSize.m_x;
        init(rpSize);
        Check();
    }

    Vector2D(RasterPoint const& rpSize, UNIT const initVal)
    {
        m_width = rpSize.m_x;
        m_rows.reserve(rpSize.m_y);
        for (int i = 0; i < rpSize.m_y; ++i)
            m_rows.push_back(make_unique<ROW>(rpSize.m_x, initVal));
        Check();
    }

    void Check() const
    {
#ifdef _DEBUG
        for (int i = 0; i < m_rows.size(); ++i)
        {
            size_t siz = m_rows[i]->m_vector.size();
            if (m_rows[i]->m_vector.size() != m_width)
            {
                int x = 42;
            }
        }
#endif
    }

    void Resize(RasterPoint const& rpSize)
    {
        if (rpSize != GetSize())
        {
            m_width = rpSize.m_x;
            m_rows.clear();
            init(rpSize);
            Check();
        }
    }

    RasterPoint GetSize() const
    {
        return RasterPoint(Width(), Height());
    }

    ROW* GetRowPtr(RasterIndex const ry)
    {
        if (ry < m_rows.size())
            return m_rows[ry].get();
        else 
            return nullptr;
    }

    ROW const * GetConstRowPtr(RasterIndex const ry) const
    {
        if (ry < m_rows.size())
            return m_rows[ry].get();
        else 
            return nullptr;
    }

    void Set(RasterPoint const& rp, UNIT val)
    {
        if (IsValid(rp))
            GetRowPtr(rp.m_y)->m_vector[rp.m_x] = val;
    }

    void Set(UNIT const newVal)
    {
        Apply2AllPixels([newVal](UNIT& val) { val = newVal; });
    }

    UNIT& GetRef(RasterPoint const& rp)
    {
        assert(IsValid(rp));
        return GetRowPtr(rp.m_y)->m_vector[rp.m_x];
    }

    UNIT const& GetConstRef(RasterPoint const& rp) const
    {
        assert(IsValid(rp));
        return GetConstRowPtr(rp.m_y)->m_vector[rp.m_x];
    }

    UNIT* GetPtr(RasterPoint const& rp)
    {
        return IsValid(rp) ? &GetRef(rp) : nullptr;
    }

    UNIT const * GetConstPtr(RasterPoint const& rp) const
    {
        return IsValid(rp) ? &GetConstRef(rp) : nullptr;
    }

    UNIT Get(RasterPoint const& rp) const
    {
        if (UNIT const * pUnit{ GetConstPtr(rp) })
            return *pUnit;
        else
            return UNIT::NULL_VAL();
    }

    UNIT Get(ROW const &row, RasterIndex const rx) const
    {
        return row.at(rx);
    }

    RasterIndex Width() const
    {
        return m_rows.empty() ? 0 : Cast2Int(m_rows.at(0)->m_vector.size());
    }

    RasterIndex Height() const
    {
        return Cast2Int(m_rows.size());
    }

    size_t NrOfPoints() const
    {
        return (m_rows.size() > 0) ? Width() * Height() : 0;
    }

    bool IsValid(RasterPoint const &pnt) const
    {
        return (0 <= pnt.m_x) && (pnt.m_x < Width()) &&
               (0 <= pnt.m_y) && (pnt.m_y < Height());
    }

    Vector2D& operator*= (float const factor)
    {
        Apply2AllRows([factor](ROW &vec) { vec *= factor; });
        return *this;
    }

    Vector2D& operator+= (Vector2D const &rhs)
    {
        assert(Height() == rhs.Height());
        for (int i = 0; i < Height(); ++i)
            *m_rows[i].get() += *rhs.m_rows[i].get();
        return *this;
    }

    void Divide(auto const &func)
    {
	    VisitAllPixels
	    (
		    [this, &func](RasterPoint const &pnt)
		    {
			    if (auto const div { func(pnt) })
				    GetRef(pnt) /= Cast2Float(div);
		    }
	    );
    }

    void Apply2AllRows(auto const& func)
    {
        Check();
        for (auto& upRow : m_rows)
            func(*upRow.get());
    }

    void Apply2AllRowsC(auto const& func) const
    {
        Check();
        for (auto const& upRow : m_rows)
            func(*upRow.get());
    }

    void Apply2AllPixels(auto const& func)
    {
        Check();
        for (auto& upRow : m_rows)
            for (UNIT& val : upRow->m_vector)
                func(val);
    }

    void Apply2AllPixelsC(auto const& func) const
    {
        Check();
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

    void VisitAllPixels(auto const& func)
    {
        RasterPoint rp;
        for (rp.m_y = 0; rp.m_y < Height(); ++rp.m_y)
        for (rp.m_x = 0; rp.m_x < Width (); ++rp.m_x)
        {
            func(rp);
        }
    }

    void VisitAllPixelsC(auto const& func) const
    {
        RasterPoint rp;
        for (rp.m_y = 0; rp.m_y < Height(); ++rp.m_y)
        for (rp.m_x = 0; rp.m_x < Width (); ++rp.m_x)
        {
            func(rp);
        }
    }

    unique_ptr<Vector2D> MeanFilter() const
    {
        assert(m_rows.size() > 0);
        unique_ptr<Vector2D> dst { make_unique<Vector2D>(GetSize()) };
        VisitAllPixelsC([this, &dst](RasterPoint const& rp){ dst->Set(rp, getMeanFiltered(rp)); });
        dst->Normalize();
        return move(dst);
    }

    unique_ptr<Vector2D> MedianFilter() const
    {
        assert(m_rows.size() > 0);
        unique_ptr<Vector2D> dst { make_unique<Vector2D>(GetSize()) };
        VisitAllPixelsC([this, &dst](RasterPoint const& rp) { dst->Set(rp, getMedianFiltered(rp)); });
        dst->Normalize();
        return move(dst);
    }

private:

    void visitNeighbours
    (
        RasterPoint const& rp,
        auto const & func
    ) const
    {
        vector<UNIT> neighbours;
        for (RasterIndex dy = -1; dy <= 1; ++dy)
        for (RasterIndex dx = -1; dx <= 1; ++dx)
        {
            RasterPoint neighbour { rp.m_x + dx, rp.m_y + dy };
            if (IsValid(neighbour))
                func(neighbour);
        }
    }

    UNIT getMeanFiltered(RasterPoint const& rp) const
    {
        UNIT  mean  = UNIT::ZERO_VAL();
        float count = 0;
        visitNeighbours
        (
            rp,
            [&](RasterPoint const& neighbour)
            {
                mean += Get(neighbour);
                ++count;
            }
        );
        return mean/count;
    }

    UNIT getMedianFiltered(RasterPoint const& rp) const
    {
        vector<UNIT> neighbours;
        visitNeighbours(rp, [&](RasterPoint const& n){ neighbours.push_back(Get(n)); });
        sort(neighbours);
        return neighbours.at(neighbours.size()/2);
    }

    vector<unique_ptr<ROW>> m_rows;
    size_t                  m_width;
};
