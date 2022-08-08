// DrawContext.h
//
// Utilities

#pragma once

#include <string>
#include "d2d1helper.h"
#include "BasicTypes.h"

import Uniform2D;
import MoreTypes;
import PixelTypes;

using std::wstring;
using D2D1::ColorF;

class DrawContext
{
public:
	virtual ~DrawContext() = default;

	void Initialize()
	{
		m_coord.Reset();
	}

	virtual void DrawLine
	(
		MicroMeterPnt const &, 
		MicroMeterPnt const &, 
		MicroMeter    const, 
		ColorF        const,
		fPixel        const = 1._fPixel
	) const = 0;

	virtual void DrawLine
	(
		MicroMeterLine const &, 
		MicroMeter     const, 
		ColorF         const
	) const = 0;

	virtual void FillArrow
	(
		MicroMeterPnt const &, 
		MicroMeterPnt const &, 
		MicroMeter    const, 
		MicroMeter    const, 
		ColorF        const
	) const = 0;

	virtual void FillCircle
	(
		MicroMeterCircle const &, 
		ColorF           const,
		fPixel           const = 1._fPixel
	) const = 0;

	virtual void DrawCircle        (MicroMeterCircle  const &, ColorF const, MicroMeter const) const = 0;
	virtual void DrawEllipse       (MicroMeterEllipse const &, ColorF const, MicroMeter const) const = 0;
	virtual void FillEllipse       (MicroMeterEllipse const &, ColorF const) const = 0;
	virtual void FillRectangle     (MicroMeterRect    const &, ColorF const) const = 0;
	virtual void FillGradientCircle(MicroMeterCircle  const &, ColorF const, ColorF const) const = 0;
	virtual void DrawTranspRect    (MicroMeterRect    const &, ColorF const) const = 0;
	virtual void DisplayText
	(
		MicroMeterRect      const &,
		wstring             const &,
		ColorF              const,
		IDWriteTextFormat * const = nullptr
	) const = 0;

	Uniform2D<MicroMeter> const & GetCoordC() const { return m_coord; }
	Uniform2D<MicroMeter>       & GetCoord ()       { return m_coord; }

	MicroMeter  GetPixelSize  () const { return m_coord.GetPixelSize  (); }
	fPixelPoint GetPixelOffset() const { return m_coord.GetPixelOffset(); }

	void SetPixelSize  (MicroMeter    const s, bool const b = true) { m_coord.SetPixelSize  (s, b); }
	void SetPixelOffset(fPixelPoint   const f, bool const b = true) { m_coord.SetPixelOffset(f, b); }
	bool Zoom          (MicroMeter    const s, bool const b = true) { return m_coord.Zoom(s, b); }
	void Center        (MicroMeterPnt const u, fPixelPoint const f, bool const b = true) { m_coord.Center(u, f, b); }

	bool Zoom(bool const bDirection, fPixelPoint const & fPixPointCenter)
	{
		return m_coord.Zoom(bDirection, fPixPointCenter);
	}

	void Move(auto const d) { m_coord.Move(d); }

	void SetNoColors(bool const bMode) { m_bNoColors = bMode; }

	bool IsTooSmall(MicroMeterRect const & rect) const
	{
		MicroMeter const umMin { m_coord.Transform2logUnit(10._fPixel) };
		return (rect.GetHeight() < umMin) || (rect.GetWidth() < umMin);
	}

protected:
	bool                  m_bNoColors { false };
	Uniform2D<MicroMeter> m_coord     { };
};