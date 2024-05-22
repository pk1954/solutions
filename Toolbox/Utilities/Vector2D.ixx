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

export template <typename UNIT>
class Vector2D
{
public:

    void init(RasterPoint const& rpSize)
    {
        m_size = rpSize;
        m_data.resize(NrOfPoints());
    }

    void init
    (
        RasterPoint const& rpSize,
        UNIT        const& val
    )
    {
        m_size = rpSize;
        m_data.resize(NrOfPoints(), val);
    }

    Vector2D() {}

    Vector2D(RasterPoint const& rpSize)
    {
        init(rpSize);
    }

    Vector2D(RasterPoint const& rpSize, UNIT const initVal)
    {
        init(rpSize, initVal);
    }

    RasterPoint Size()        const { return m_size; }
    RasterIndex Width ()      const { return m_size.m_x; }
    RasterIndex Height()      const { return m_size.m_y; }
    size_t      NrOfPoints()  const { return Width() * Height(); }
    float       AspectRatio() const { return static_cast<float>(Height()) / static_cast<float>(Width()); }

    void Resize(RasterPoint const& rpSize)
    {
        if (rpSize != Size())
            init(rpSize);
    }

    void Set(RasterPoint const& rp, UNIT val)
    {
        if (IsValid(rp))
            GetRef(rp) = val;
    }

    void Set(UNIT const newVal)
    {
        Apply2AllPixels([newVal](UNIT& val) { val = newVal; });
    }

    UNIT& GetRef(RasterPoint const& rp)
    {
        assert(IsValid(rp));
        return m_data[rp.m_y * Width() + rp.m_x];
    }

    UNIT const& GetConstRef(RasterPoint const& rp) const
    {
        assert(IsValid(rp));
        return m_data[rp.m_y * Width() + rp.m_x];
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
        UNIT const* pUnit{ GetConstPtr(rp) };
        assert(pUnit);
        return *pUnit;
    }

    bool IsValid(RasterPoint const &pnt) const
    {
        return (0 <= pnt.m_x) && (pnt.m_x < m_size.m_x) &&
               (0 <= pnt.m_y) && (pnt.m_y < m_size.m_y);
    }

    Vector2D& operator*= (float const factor)
    {
        for (UNIT& val : m_data)
            val = static_cast<UNIT>(val * factor);
        return *this;
    }

    Vector2D& operator+= (Vector2D const &rhs)
    {
        assert(m_size == rhs.m_size);
	    m_size.VisitAllRasterPoints
	    (
		    [this, &rhs](RasterPoint const &rp)
		    {
			    GetRef(rp) += rhs.GetConstRef(rp);
		    }
	    );
        return *this;
    }

    void Divide(auto const &func)
    {
	    m_size.VisitAllRasterPoints
	    (
		    [this, &func](RasterPoint const &pnt)
		    {
			    if (auto const div { func(pnt) })
				    GetRef(pnt) /= Cast2Float(div);
		    }
	    );
    }

    void Apply2AllPixels(auto const& func)
    {
        for (auto& it : m_data)
            func(it);
    }

    void Apply2AllPixelsC(auto const& func) const
    {
        for (auto const& it : m_data)
            func(it);
    }

    UNIT CalcMaxValue() const
    {
        UNIT valMax { 0 };
        for (auto const& it: m_data)  
            { if (it > valMax) valMax = it; }
        return valMax;
    }

    void Normalize(float const fMax)
    {
		if (UNIT unitMax { CalcMaxValue() })
			*this *= fMax / unitMax;
    }

    unique_ptr<Vector2D> MeanFilter() const
    {
        assert(Height() > 0);
        unique_ptr<Vector2D> dst { make_unique<Vector2D>(Size()) };
        m_size.VisitAllRasterPointsC([this, &dst](RasterPoint const& rp){ dst->Set(rp, getMeanFiltered(rp)); });
        return dst;
    }

    unique_ptr<Vector2D> MedianFilter() const
    {
        assert(Height() > 0);
        unique_ptr<Vector2D> dst { make_unique<Vector2D>(Size()) };
        m_size.VisitAllRasterPointsC([this, &dst](RasterPoint const& rp) { dst->Set(rp, getMedianFiltered(rp)); });
        return dst;
    }

private:

    void visitNeighbours
    (
        RasterPoint const& rp,
        auto        const& func
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
        float fCount { 0.0f };
        UNIT  fMean  { UNIT::ZERO_VAL() };
        visitNeighbours
        (
            rp,
            [&](RasterPoint const& neighbour)
            {
                fMean += Get(neighbour);
                ++fCount;
            }
        );
        return fMean/fCount;
    }

    UNIT getMedianFiltered(RasterPoint const& rp) const
    {
        vector<UNIT> neighbours;
        visitNeighbours(rp, [&](RasterPoint const& n){ neighbours.push_back(Get(n)); });
        sort(neighbours);
        return neighbours.at(neighbours.size()/2);
    }

    RasterPoint  m_size;
    vector<UNIT> m_data;
};
