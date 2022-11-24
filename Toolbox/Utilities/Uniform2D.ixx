// Uniform2D.ixx 
//
// ToolBox\Utilities

module;

#include <iostream>
#include <algorithm>

export module Uniform2D;

import Util;
import IoConstants;
import Observable;
import PixFpDimension;
import Types;

using std::max;
using std::wostream;

export template <typename LOG_UNIT>
class Uniform2D : public Observable
{
public:

	void Reset()
	{
		m_xDim.SetPixelSize(DEFAULT_PIXEL_SIZE);
		m_yDim.SetPixelSize(DEFAULT_PIXEL_SIZE);
		m_xDim.SetOffset(0.0_fPixel);
		m_yDim.SetOffset(0.0_fPixel);
	}

	//////// transformations LOG_UNIT <---> fPixel ////////

	fPixel Transform2fPixel(LOG_UNIT const param) const
	{ 
		return fPixel(param / pixelSize());
	}

	LOG_UNIT Transform2logUnit(fPixel const fPixel) const
	{ 
		return pixelSize() * fPixel.GetValue();
	}

	fPixelPoint Transform2fPixelSize(PosType<LOG_UNIT> const np) const
	{ 
		return fPixelPoint(Transform2fPixel(np.GetX()), Transform2fPixel(np.GetY()));
	}

	fPixelRectSize Transform2fPixelRectSize(SizeType<LOG_UNIT> const np) const
	{ 
		return fPixelRectSize(Transform2fPixel(np.GetX()), Transform2fPixel(np.GetY()));
	}

	PosType<LOG_UNIT> Transform2logUnitPntSize(fPixelPoint const pp) const
	{ 
		return PosType<LOG_UNIT>(Transform2logUnit(pp.GetX()), Transform2logUnit(pp.GetY())); 
	}

	SizeType<LOG_UNIT> Transform2logUnitRectSize(fPixelRectSize const pp) const
	{ 
		return SizeType<LOG_UNIT>(Transform2logUnit(pp.GetX()), Transform2logUnit(pp.GetY())); 
	}

	PosType<LOG_UNIT> Transform2logUnitPntPos(fPixelPoint const pp) const
	{ 
		return Transform2logUnitPntSize(pp + GetPixelOffset());
	}

	PosType<LOG_UNIT> Transform2logUnitPntPos(PixelPoint const pixPoint) const
	{ 
		return Transform2logUnitPntSize(::Convert2fPixelPoint(pixPoint) + GetPixelOffset());
	}

	fPixelPoint Transform2fPixelPos(PosType<LOG_UNIT> const np) const
	{ 
		return Transform2fPixelSize(np) - GetPixelOffset();
	}

	RectType<LOG_UNIT> Transform2logUnitRect(fPixelRect const & fPixRect) const
	{ 
		return RectType<LOG_UNIT>
			(
				Transform2logUnitPntPos(fPixRect.GetStartPoint()), 
				Transform2logUnitPntPos(fPixRect.GetEndPoint  ()) 
			);
	}

	fPixelRect Transform2fPixelRect(RectType<LOG_UNIT> const & umRect) const
	{
		return fPixelRect
		(
			Transform2fPixelPos(umRect.GetStartPoint()), 
			Transform2fPixelPos(umRect.GetEndPoint  ()) 
		);
	}

	CircleType<LOG_UNIT> Transform2logUnitCircle(fPixelCircle const & fPixCircle) const
	{
		return CircleType<LOG_UNIT>
			(
				Transform2logUnitPntPos(fPixCircle.GetPos()),
				Transform2logUnit      (fPixCircle.GetRadius  ())
				);
	}

	fPixelCircle Transform2fPixelCircle(CircleType<LOG_UNIT> const & umCircle) const
	{
		return fPixelCircle
		(
			Transform2fPixelPos(umCircle.GetPos()),
			Transform2fPixel   (umCircle.GetRadius())
		);
	}

	fPixelEllipse Transform2fPixelEllipse(EllipseType<LOG_UNIT> const & umEllipse) const
	{
		return fPixelEllipse
		(
			Transform2fPixelPos(umEllipse.GetPos()),
			Transform2fPixel   (umEllipse.GetRadiusX()),
			Transform2fPixel   (umEllipse.GetRadiusY())
		);
	}

	//////// transformations PIXEL <---> fPixel ////////

	PixelRect Transform2PixelRect(fPixelRect const & fPixRect) const
	{
		return PixelRect
		(
			::Convert2PixelPoint(fPixRect.GetStartPoint()),
			::Convert2PixelPoint(fPixRect.GetEndPoint  ())
		);
	}

	fPixelRect Transform2fPixelRect(PixelRect const & rect) const
	{
		return fPixelRect
		(
			::Convert2fPixelPoint(rect.GetStartPoint()), 
			::Convert2fPixelPoint(rect.GetEndPoint  ()) 
		);
	}

	fPixelRectSize Transform2fPixelRectSize(PixelRectSize const & siz) const
	{
		return fPixelRectSize
		(
			::Convert2fPixel(siz.GetX()), 
			::Convert2fPixel(siz.GetY()) 
		);
	}

	//////// transformations LOG_UNIT <---> PIXEL  ////////

	LOG_UNIT Transform2logUnit(PIXEL const pix) const
	{ 
		return Transform2logUnit(::Convert2fPixel(pix));
	}

	PosType<LOG_UNIT> Transform2logUnitPntSize(PixelPoint const & pnt) const
	{ 
		return Transform2logUnitPntSize(::Convert2fPixelPoint(pnt));
	}

	SizeType<LOG_UNIT> Transform2logUnitRectSize(PixelRectSize const & siz) const
	{ 
		return Transform2logUnitRectSize(Transform2fPixelRectSize(siz));
	}

	RectType<LOG_UNIT> Transform2logUnitRect(PixelRect const & rect) const
	{ 
		return Transform2logUnitRect(Transform2fPixelRect(rect));
	}

	PIXEL Transform2PIXEL(LOG_UNIT const um) const
	{
		return ::Convert2PIXEL(Transform2fPixel(um));
	}

	PixelPoint Transform2PixelSize(PosType<LOG_UNIT> const & umPnt) const
	{
		return ::Convert2PixelPoint(Transform2fPixelSize(umPnt));
	}

	PixelPoint Transform2PixelPos(PosType<LOG_UNIT> const & umPnt) const
	{
		return ::Convert2PixelPoint(Transform2fPixelPos(umPnt));
	}

	PixelRect Transform2PixelRect(RectType<LOG_UNIT> const & umRect) const
	{
		return Transform2PixelRect(Transform2fPixelRect(umRect));
	}

	//////// queries ////////

	LOG_UNIT GetPixelSize() const 
	{ 
		return m_xDim.GetPixelSize(); 
	}

	fPixelPoint GetPixelOffset() const 
	{ 
		return fPixelPoint(m_xDim.GetPixelOffset(), m_yDim.GetPixelOffset()); 
	}

	//////// manipulation functions ////////

	void SetPixelSize(LOG_UNIT const logSize, bool const bNotify = true)
	{ 
		m_xDim.SetPixelSize(logSize, false);
		m_yDim.SetPixelSize(logSize, false);
		if (bNotify)
			NotifyAll(true);
	}

	void SetPixelOffset(fPixelPoint const fPixOffset, bool const bNotify = true)
	{ 
		m_xDim.SetOffset(fPixOffset.GetX(), false);
		m_yDim.SetOffset(fPixOffset.GetY(), false);
		if (bNotify)
			NotifyAll(true);
	}

	void Set(Uniform2D const & newVals) 
	{ 
		* this = newVals; 
	}

	void Move(PixelPoint const & pixPntDelta) 
	{ 
		m_xDim.Move(Convert2fPixel(pixPntDelta.GetX()));
		m_yDim.Move(Convert2fPixel(pixPntDelta.GetY()));
	}

	void Move(PosType<LOG_UNIT> const & logPntDelta) 
	{ 
		m_xDim.Move(Transform2fPixel(logPntDelta.GetX()));
		m_yDim.Move(Transform2fPixel(logPntDelta.GetY()));
	}

	bool Zoom(LOG_UNIT const logSize, bool const bNotify = true)
	{
		bool bValid = IsValidPixelSize(logSize);
		if (bValid)
			SetPixelSize(logSize, bNotify);
		return bValid;
	}

	void Center
	(
		PosType<LOG_UNIT> const logPntCenter,   
		fPixelPoint       const fPntPix,
		bool              const bNotify = true
	)
	{
		SetPixelOffset(Transform2fPixelSize(logPntCenter) - fPntPix, bNotify);
	}

	bool ZoomCenter
	(
		float       const fFactor,
		fPixelPoint const fPixPntCenter,
		bool        const bNotify = true
	)
	{
		bool const bResX = m_xDim.ZoomCenter(fFactor, fPixPntCenter.GetX(), false);
		bool const bResY = m_yDim.ZoomCenter(fFactor, fPixPntCenter.GetY(), false);
		return bResX && bResY;
	}

	bool Zoom(bool const bDirection, fPixelPoint const & fPixPointCenter)
	{
		bool const bResX = m_xDim.ZoomDir(bDirection, fPixPointCenter.GetX(), false);
		bool const bResY = m_yDim.ZoomDir(bDirection, fPixPointCenter.GetY(), false);
		return bResX && bResY;
	}

	LOG_UNIT ComputeZoom
	(
		RectType<LOG_UNIT> const & umRect,     
		PixelRectSize      const   pixRectSize,
		float              const   fRatioFactor
	) const
	{
		SizeType<LOG_UNIT> const umRectSize       { Transform2logUnitRectSize(pixRectSize) };
		float              const fVerticalRatio   { umRect.GetHeight() / umRectSize.GetY() };
		float              const fHorizontalRatio { umRect.GetWidth () / umRectSize.GetX() };
		float              const fMaxRatio        { max(fVerticalRatio, fHorizontalRatio) };
		float              const fDesiredRatio    { fMaxRatio * fRatioFactor };
		LOG_UNIT           const sizeDesired      { GetPixelSize() * fDesiredRatio };
		return ClipToMinMax<LOG_UNIT>(sizeDesired, MINIMUM_PIXEL_SIZE, MAXIMUM_PIXEL_SIZE);
	}

	Uniform2D operator+= (Uniform2D const & a) 
	{ 
		m_xDim += a.m_xDim;
		m_yDim += a.m_yDim;
		return * this; 
	}

	Uniform2D operator-= (Uniform2D const & a) 
	{ 
		m_xDim -= a.m_xDim;
		m_yDim -= a.m_yDim;
		return * this; 
	}

	Uniform2D operator*= (float const factor) 
	{ 
		m_xDim *= factor;
		m_yDim *= factor; 
		return * this; 
	}

	friend Uniform2D operator+ (Uniform2D const & a, Uniform2D const & b) 
	{ 
		Uniform2D res { a }; 
		res += b; 
		return res; 
	};

	friend Uniform2D operator- (Uniform2D const & a, Uniform2D const & b) 
	{ 
		Uniform2D res { a }; 
		res -= b; 
		return res; 
	};

	friend Uniform2D operator* (Uniform2D const & a, float const factor) 
	{ 
		Uniform2D res { a }; 
		res *= factor; 
		return res; 
	};

	bool IsValidPixelSize(LOG_UNIT const size) const
	{
		return (MINIMUM_PIXEL_SIZE <= size) && (size <= MAXIMUM_PIXEL_SIZE); 
	}

	friend wostream& operator<< (wostream& out, Uniform2D<LOG_UNIT> const& uniform2D)
	{
		out << OPEN_BRACKET << uniform2D.m_xDim
			<< SEPARATOR << uniform2D.m_yDim
			<< CLOSE_BRACKET;
		return out;
	}

private:

	inline static LOG_UNIT const MINIMUM_PIXEL_SIZE { LOG_UNIT(   0.1f) };
	inline static LOG_UNIT const DEFAULT_PIXEL_SIZE { LOG_UNIT(   1.0f) };  
	inline static LOG_UNIT const MAXIMUM_PIXEL_SIZE { LOG_UNIT(2000.0f) };  // 2 MilliMeter

	PixFpDimension<LOG_UNIT> m_xDim;
	PixFpDimension<LOG_UNIT> m_yDim;

	LOG_UNIT pixelSize() const { return m_xDim.GetPixelSize(); }
};
