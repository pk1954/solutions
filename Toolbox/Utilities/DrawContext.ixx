// DrawContext.ixx
//
// Utilities

module;

#include <string>
#include "d2d1helper.h"

export module DrawContext;

import Uniform2D;
import Types;

using std::wstring;

export class DrawContext
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
		D2D1::ColorF  const,
		fPixel        const = 1._fPixel
	) const = 0;

	virtual void DrawLine
	(
		MicroMeterLine const &, 
		MicroMeter     const, 
		D2D1::ColorF   const
	) const = 0;

	virtual void FillArrow
	(
		MicroMeterPnt const &, 
		MicroMeterPnt const &, 
		MicroMeter    const, 
		MicroMeter    const, 
		D2D1::ColorF  const
	) const = 0;

	virtual void FillCircle
	(
		MicroMeterCircle const &, 
		D2D1::ColorF     const,
		fPixel           const = 1._fPixel
	) const = 0;

	virtual void SetRotation(Degrees const, MicroMeterPnt const&) const = 0;
	virtual void Reset() const = 0;

	virtual void DrawCircle          (MicroMeterCircle  const &, D2D1::ColorF const, MicroMeter const) const = 0;
	virtual void DrawEllipse         (MicroMeterEllipse const &, D2D1::ColorF const, MicroMeter const) const = 0;
	virtual void FillEllipse         (MicroMeterEllipse const &, D2D1::ColorF const) const = 0;
	virtual void FillRectangle       (MicroMeterRect    const &, D2D1::ColorF const) const = 0;
	virtual void FillGradientCircle  (MicroMeterCircle  const &, D2D1::ColorF const, D2D1::ColorF const) const = 0;
	virtual void FillRoundedRectangle(MicroMeterRect    const &, D2D1::ColorF, MicroMeter const) const = 0;
	virtual void DrawTranspRect      (MicroMeterRect    const &, D2D1::ColorF const) const = 0;
	virtual void DisplayText
	(
		MicroMeterRect      const &,
		wstring             const &,
		D2D1::ColorF        const,
		IDWriteTextFormat * const = nullptr
	) const = 0;

	Uniform2D<MicroMeter> const & GetCoordC() const { return m_coord; }
	Uniform2D<MicroMeter>       & GetCoord ()       { return m_coord; }

	MicroMeter  GetPixelSize  () const { return m_coord.GetPixelSize  (); }
	fPixelPoint GetPixelOffset() const { return m_coord.GetPixelOffset(); }

	void SetPixelSize  (MicroMeter    const s, bool const b = true) { m_coord.SetPixelSize  (s, b); }
	void SetPixelOffset(fPixelPoint   const f, bool const b = true) { m_coord.SetPixelOffset(f, b); }
	bool Zoom          (MicroMeter    const s, bool const b = true) { return m_coord.Zoom(s, b); }
	void Center        (MicroMeterPnt const u, fPixelPoint const f, bool const b = true) 
	{ 
		m_coord.Center(u, f, b); 
	}

	bool Zoom(bool const bDirection, fPixelPoint const & fPixPointCenter)
	{
		return m_coord.Zoom(bDirection, fPixPointCenter);
	}

	void Move(auto const & d) { m_coord.Move(d); }

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