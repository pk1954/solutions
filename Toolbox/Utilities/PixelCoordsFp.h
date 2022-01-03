// PixelCoordsFp.h : 
//
// Utilities

#pragma once

#include "util.h"
#include "MoreTypes.h"
#include "PixelTypes.h"

using std::endl;

class PixelCoordsFp
{
public:

	void Reset()
	{
		m_fPixOffset = 0.0_fPixel;
		m_pixelSize  = DEFAULT_PIXEL_SIZE;
	}

	//////// transformations MicroMeter <---> fPixel ////////

	fPixel Transform2fPixel(MicroMeter const param) const
	{ 
		return fPixel(param / m_pixelSize);
	}

	MicroMeter Transform2MicroMeter(fPixel const fPixel) const
	{ 
		return m_pixelSize * fPixel.GetValue();
	}

	fPixelPoint Transform2fPixelSize(MicroMeterPnt const np) const
	{ 
		return fPixelPoint(Transform2fPixel(np.GetX()), Transform2fPixel(np.GetY()));
	}

	fPixelRectSize Transform2fPixelRectSize(MicroMeterRectSize const np) const
	{ 
		return fPixelRectSize(Transform2fPixel(np.GetX()), Transform2fPixel(np.GetY()));
	}

	MicroMeterPnt Transform2MicroMeterPntSize(fPixelPoint const pp) const
	{ 
		return MicroMeterPnt(Transform2MicroMeter(pp.GetX()), Transform2MicroMeter(pp.GetY())); 
	}

	MicroMeterRectSize Transform2MicroMeterRectSize(fPixelRectSize const pp) const
	{ 
		return MicroMeterRectSize(Transform2MicroMeter(pp.GetX()), Transform2MicroMeter(pp.GetY())); 
	}

	MicroMeterPnt Transform2MicroMeterPntPos(fPixelPoint const pp) const
	{ 
		return Transform2MicroMeterPntSize(pp + m_fPixOffset);
	}

	MicroMeterPnt Transform2MicroMeterPntPos(PixelPoint const pixPoint) const
	{ 
		return Transform2MicroMeterPntSize(::Convert2fPixelPoint(pixPoint) + m_fPixOffset);
	}

	fPixelPoint Transform2fPixelPos(MicroMeterPnt const np) const
	{ 
		return Transform2fPixelSize(np) - m_fPixOffset;
	}

	MicroMeterRect Transform2MicroMeterRect(fPixelRect const & fPixRect) const
	{ 
		return MicroMeterRect
		(
			Transform2MicroMeterPntPos(fPixRect.GetStartPoint()), 
			Transform2MicroMeterPntPos(fPixRect.GetEndPoint  ()) 
		);
	}

	fPixelRect Transform2fPixelRect(MicroMeterRect const & umRect) const
	{
		return fPixelRect
		(
			Transform2fPixelPos(umRect.GetStartPoint()), 
			Transform2fPixelPos(umRect.GetEndPoint  ()) 
		);
	}

	MicroMeterCircle Transform2MicroMeterCircle(fPixelCircle const & fPixCircle) const
	{
		return MicroMeterCircle
		(
			Transform2MicroMeterPntPos(fPixCircle.GetPos()),
			Transform2MicroMeter        (fPixCircle.GetRadius  ())
		);
	}

	fPixelCircle Transform2fPixelCircle(MicroMeterCircle const & umCircle) const
	{
		return fPixelCircle
		(
			Transform2fPixelPos(umCircle.GetPos()),
			Transform2fPixel   (umCircle.GetRadius())
		);
	}

	fPixelEllipse Transform2fPixelEllipse(MicroMeterEllipse const & umEllipse) const
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

	//////// transformations MicroMeter <---> PIXEL  ////////

	MicroMeter Transform2MicroMeter(PIXEL const pix) const
	{ 
		return Transform2MicroMeter(::Convert2fPixel(pix));
	}

	MicroMeterPnt Transform2MicroMeterPntSize(PixelPoint const & pnt) const
	{ 
		return Transform2MicroMeterPntSize(::Convert2fPixelPoint(pnt));
	}

	MicroMeterRectSize Transform2MicroMeterRectSize(PixelRectSize const & siz) const
	{ 
		return Transform2MicroMeterRectSize(Transform2fPixelRectSize(siz));
	}

	MicroMeterRect Transform2MicroMeterRect(PixelRect const & rect) const
	{ 
		return Transform2MicroMeterRect(Transform2fPixelRect(rect));
	}

	PIXEL Transform2PIXEL(MicroMeter const um) const
	{
		return ::Convert2PIXEL(Transform2fPixel(um));
	}

	PixelPoint Transform2PixelSize(MicroMeterPnt const & umPnt) const
	{
		return ::Convert2PixelPoint(Transform2fPixelSize(umPnt));
	}

	PixelPoint Transform2PixelPos(MicroMeterPnt const & umPnt) const
	{
		return ::Convert2PixelPoint(Transform2fPixelPos(umPnt));
	}

	PixelRect Transform2PixelRect(MicroMeterRect const & umRect) const
	{
		return Transform2PixelRect(Transform2fPixelRect(umRect));
	}
	
	//////// queries ////////

	MicroMeter  GetPixelSize()   const { return m_pixelSize; };
	fPixelPoint GetPixelOffset() const { return m_fPixOffset; }

	//////// manipulation functions ////////

	void SetPixelSize  (MicroMeter    const pixelSize ) { m_pixelSize  = pixelSize;  }
	void SetPixelOffset(fPixelPoint   const fPixOffset) { m_fPixOffset = fPixOffset; }
	void Set           (PixelCoordsFp const newVals   ) { * this = newVals; }

	void Move(PixelPoint    const pntDelta) { m_fPixOffset -= ::Convert2fPixelPoint(pntDelta); }
	void Move(MicroMeterPnt const umDelta ) { m_fPixOffset -= Transform2fPixelSize (umDelta); }

	bool Zoom(MicroMeter const pixelSize)
	{
		bool bValid = IsValidPixelSize(pixelSize);
		if (bValid)
			SetPixelSize(pixelSize);
		return bValid;
	}

	void Center
	(
		MicroMeterPnt const umPntCenter,   
		fPixelPoint   const fPntPix  
	)
	{
		SetPixelOffset(Transform2fPixelSize(umPntCenter) - fPntPix);
	}

	MicroMeter ComputeZoom
	(
		MicroMeterRect const & umRect,     
		PixelRectSize  const   pixRectSize,
		float          const   fRatioFactor
	) const
	{
		MicroMeterRectSize const umRectSize       { Transform2MicroMeterRectSize(pixRectSize) };
		float              const fVerticalRatio   { umRect.GetHeight() / umRectSize.GetY() };
		float              const fHorizontalRatio { umRect.GetWidth () / umRectSize.GetX() };
		float              const fMaxRatio        { max(fVerticalRatio, fHorizontalRatio) };
		float              const fDesiredRatio    { fMaxRatio * fRatioFactor };
		MicroMeter         const sizeDesired      { GetPixelSize() * fDesiredRatio };
		return ClipToMinMax<MicroMeter>(sizeDesired, MINIMUM_PIXEL_SIZE, MAXIMUM_PIXEL_SIZE);
	}

	PixelCoordsFp operator+= (PixelCoordsFp const a) 
	{ 
		m_fPixOffset += a.m_fPixOffset;
		m_pixelSize  += a.m_pixelSize; 
		return * this; 
	}

	PixelCoordsFp operator-= (PixelCoordsFp const a) 
	{ 
		m_fPixOffset -= a.m_fPixOffset;
		m_pixelSize  -= a.m_pixelSize; 
		return * this; 
	}

	PixelCoordsFp operator*= (float const factor) 
	{ 
		m_fPixOffset *= factor;
		m_pixelSize  *= factor; 
		return * this; 
	}

	friend PixelCoordsFp operator+ (PixelCoordsFp const a, PixelCoordsFp const b) 
	{ 
		PixelCoordsFp res { a }; 
		res += b; 
		return res; 
	};

	friend PixelCoordsFp operator- (PixelCoordsFp const a, PixelCoordsFp const b) 
	{ 
		PixelCoordsFp res { a }; 
		res -= b; 
		return res; 
	};

	friend PixelCoordsFp operator* (PixelCoordsFp const a, float const factor) 
	{ 
		PixelCoordsFp res { a }; 
		res *= factor; 
		return res; 
	};

	friend wostream & operator<< (wostream & out, PixelCoordsFp const & c)
	{
		out << c.m_fPixOffset << L", " << c.m_pixelSize << endl;
		return out; 
	}

	bool IsValidPixelSize(MicroMeter const size) const
	{
		return (MINIMUM_PIXEL_SIZE <= size) && (size <= MAXIMUM_PIXEL_SIZE); 
	}

private:

	inline static MicroMeter const MINIMUM_PIXEL_SIZE {    0.1_MicroMeter };
	inline static MicroMeter const DEFAULT_PIXEL_SIZE {    1.0_MicroMeter };  
	inline static MicroMeter const MAXIMUM_PIXEL_SIZE { 2000.0_MicroMeter };  // 2 MilliMeter

	fPixelPoint m_fPixOffset { 0.0_fPixel };
	MicroMeter  m_pixelSize  { DEFAULT_PIXEL_SIZE };
};
