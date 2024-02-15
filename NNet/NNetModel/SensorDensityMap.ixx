// SensorDensityMap.ixx
//
// NNetModel

module;

#include <optional>
#include <cassert>

export module NNetModel:SensorDensityMap;

import Util;
import SaveCast;
import ColorLUT;
import Vector2D;
import Types;
import Raster;
import DrawContext;
import :UPNobList;
import :Pipe;

using std::optional;

export using PntCount = unsigned short;

export class SensorDensityMap
{
public:
    bool IsValid(RasterPoint const& rp) const 
    { 
        return m_map.IsValid(rp); 
    }

    void FillMap
    (
        Raster    const& raster,
        UPNobList const& list
    )
    {
        m_map.Resize(raster.Size(), 0);
        list.Apply2AllC<Pipe>
        (
            [this, raster](Pipe const& p) { addSensorPoints(p, raster); }
        );
        m_maxCnt = 0;
        m_sumCnt = 0;
        m_map.Apply2AllC
        (
            [this](PntCount const& cnt) 
            { 
                m_sumCnt += cnt;
                if (cnt > m_maxCnt) 
                    m_maxCnt = cnt; 
            }
        );
    }

    PntCount GetNrOfDataPoints(RasterPoint const& rp) const
    {
        return (IsValid(rp))
            ? m_map.Get(rp)
            : 0;
    }

    float GetAverageDataPoints(Raster const& raster) const
    {
        float const fNrOfRasterPnts  { Cast2Float(raster.NrOfElements()) };
	    float const fAverageDataPnts { Cast2Float(m_sumCnt) / fNrOfRasterPnts};
	    return fAverageDataPnts;
    }

    float GetMaxDataPoints() const
    {
	    return Cast2Float(m_maxCnt);
    }

    void Draw
    (
        Raster      const& raster,
        UPNobList   const& list,
        DrawContext const& drawContext
    )
    {
        FillMap(raster, list);
	    if (m_maxCnt == 0)
		    return;

	    ColorLUT const lut { sensorDensityLUT(raster) };
	    raster.DrawRasterPoints
	    (
		    drawContext, 
		    [this, &lut](auto const &rp)->Color { return getColor(rp, lut); }
	    );
    }

private:

    void addSensorPoints
    (
        Pipe   const& pipe,
        Raster const& raster
    )
    {
        pipe.Apply2AllSensorPoints
        (
            raster.Resolution(),
            [this, &raster](Pipe const& pipe, MicroMeterPnt const& umPnt, Pipe::SegNr const segNr)
            {
                if (optional<RasterPoint> const rPntOpt = raster.FindRasterPos(umPnt))
                {
                    PntCount* pCnt { m_map.GetPtr(rPntOpt.value()) };
                    assert(pCnt);
                    ++*pCnt;
                }
            }
        );
    }

    ColorLUT sensorDensityLUT(Raster const& raster) const
    {
        float const fNrOfRasterPnts  { Cast2Float(raster.NrOfElements()) };
	    float const fAverageDataPnts { Cast2Float(m_sumCnt) / fNrOfRasterPnts};
	    float const fAverage         { 255.0f * fAverageDataPnts / Cast2Float(m_maxCnt) };
        float const fLutIndexAverage { ClipToMinMax(fAverage, 1.0f, 254.0f) };
        int   const indexAverage     { Cast2Int(fLutIndexAverage + 0.5f) };
	    ColorLUT lut;
        lut.AddBasePoint(           0, Color(D2D1::ColorF::Black));
	    lut.AddBasePoint(indexAverage, Color(D2D1::ColorF::Green));
        lut.AddBasePoint(         255, Color(D2D1::ColorF::Red));
        lut.Construct();
	    return lut;
    }

    Color getColor
    (
        RasterPoint const& rp,
        ColorLUT    const& lut
    )
    {
		PntCount const nrOfPnts { m_map.Get(rp) };
        assert(nrOfPnts <= m_maxCnt);
		unsigned int const lutIndex { (255 * static_cast<unsigned long>(nrOfPnts)) / m_maxCnt };
		return lut.Get(Cast2Int(lutIndex));
	}

    Vector2D<PntCount> m_map;
    PntCount           m_maxCnt;
    PntCount           m_sumCnt;
};