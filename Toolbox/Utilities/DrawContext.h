// DrawContext.h
//
// Utilities

#pragma once

#include "d2d1helper.h"
#include "PixelTypes.h"
#include "PixelCoordsFp.h"

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

	PixelCoordsFp const & GetCoordC() const { return m_coord; }
	PixelCoordsFp       & GetCoord ()       { return m_coord; }

	MicroMeter  GetPixelSize  () const { return m_coord.GetPixelSize  (); }
	fPixelPoint GetPixelOffset() const { return m_coord.GetPixelOffset(); }

	void SetPixelSize  (MicroMeter    const s) { m_coord.SetPixelSize  (s); }
	void SetPixelOffset(fPixelPoint   const f) { m_coord.SetPixelOffset(f); }
	bool Zoom          (MicroMeter    const s) { return m_coord.Zoom   (s); }
	void Center        (MicroMeterPnt const u, fPixelPoint const f) { m_coord.Center(u, f); }

	void Move(auto const d) { m_coord.Move(d); }

	void SetNoColors(bool const bMode) { m_bNoColors = bMode; }

	bool IsTooSmall(MicroMeterRect const & rect) const
	{
		MicroMeter const umMin { m_coord.Transform2MicroMeter(10._fPixel) };
		return (rect.GetHeight() < umMin) || (rect.GetWidth() < umMin);
	}

protected:
	bool          m_bNoColors { false };
	PixelCoordsFp m_coord     { };
};