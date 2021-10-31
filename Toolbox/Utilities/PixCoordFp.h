// PixCoordFp.h : 
//
// Utilities

#pragma once

#include "util.h"
#include "MoreTypes.h"
#include "PixelTypes.h"

using std::endl;

template <typename LOG_UNIT>
class PixCoordFp
{
public:

	PixCoordFp()
	  : m_fPixOffset(0.0_fPixel),
		m_pixelSize (1.0f)
	{}

	PixCoordFp
	(	
		fPixel   const fPixOffset,
		LOG_UNIT const pixelSize
	)
	  : m_fPixOffset(fPixOffset),
		m_pixelSize ()
	{}

	void Reset()
	{
		m_fPixOffset = 0.0_fPixel;
		m_pixelSize  = LOG_UNIT(1.0f);
	}

	//////// transformations LOG_UNIT <---> fPixel ////////

	fPixel const Transform2fPixel(LOG_UNIT const param) const
	{ 
		return fPixel(param / m_pixelSize);
	}

	LOG_UNIT const Transform2logUnit(fPixel const fPixel) const
	{ 
		return m_pixelSize * fPixel.GetValue();
	}

	LOG_UNIT const Transform2logUnitPos(PIXEL const pix) const
	{ 
		return Transform2logUnit(::Convert2fPixel(pix) + m_fPixOffset);
	}

	fPixel const Transform2fPixelPos(LOG_UNIT const np) const
	{ 
		return Transform2fPixel(np) - m_fPixOffset;
	}

	//////// transformations LOG_UNIT <---> PIXEL  ////////

	LOG_UNIT Transform2logUnit(PIXEL const pix) const
	{ 
		return Transform2logUnit(::Convert2fPixel(pix));
	}

	PIXEL Transform2PIXEL(LOG_UNIT const um) const
	{
		return ::Convert2PIXEL(Transform2fPixel(um));
	}

	PIXEL Transform2PixelPos(LOG_UNIT const & umPnt) const
	{
		return ::Convert2PIXEL(Transform2fPixelPos(umPnt));
	}

	//////// queries ////////

	LOG_UNIT const GetPixelSize()   const { return m_pixelSize; };
	fPixel   const GetPixelOffset() const { return m_fPixOffset; }

	//////// manipulation functions ////////

//	void Set(PixCoordFp const newVals ) { * this = newVals; }

	void Move(PIXEL    const pixDelta) { m_fPixOffset -= ::Convert2fPixel(pixDelta); }
	void Move(LOG_UNIT const umDelta ) { m_fPixOffset -= Transform2fPixel(umDelta); }

	bool Zoom(LOG_UNIT const pixelSize)
	{
		bool bValid = IsValidPixelSize(pixelSize);
		if (bValid)
			SetPixelSize(pixelSize);
		return bValid;
	}

	void Center
	(
		LOG_UNIT const umCenter,   
		fPixel     const fPntPix  
	)
	{
		SetPixelOffset(Transform2fPixel(umCenter) - fPntPix);
	}

	PixCoordFp const operator+= (PixCoordFp const a) 
	{ 
		m_fPixOffset += a.m_fPixOffset;
		m_pixelSize  += a.m_pixelSize; 
		return * this; 
	}

	PixCoordFp const operator-= (PixCoordFp const a) 
	{ 
		m_fPixOffset -= a.m_fPixOffset;
		m_pixelSize  -= a.m_pixelSize; 
		return * this; 
	}

	PixCoordFp const operator*= (float const factor) 
	{ 
		m_fPixOffset *= factor;
		m_pixelSize  *= factor; 
		return * this; 
	}

	friend PixCoordFp const operator+ (PixCoordFp const a, PixCoordFp const b) 
	{ 
		PixCoordFp res { a }; 
		res += b; 
		return res; 
	};

	friend PixCoordFp const operator- (PixCoordFp const a, PixCoordFp const b) 
	{ 
		PixCoordFp res { a }; 
		res -= b; 
		return res; 
	};

	friend PixCoordFp const operator* (PixCoordFp const a, float const factor) 
	{ 
		PixCoordFp res { a }; 
		res *= factor; 
		return res; 
	};

	//friend wostream & operator<< (wostream & out, fPixel const & c)
	//{
	//	out << c.m_fPixOffset << L", " << c.m_pixelSize << endl;
	//	return out; 
	//}

	bool IsValidPixelSize(LOG_UNIT const size) const
	{
		return (m_pixelSizeMin <= size) && (size <= m_pixelSizeMax); 
	}

	void SetPixelSize(LOG_UNIT const s) { m_pixelSize = s; }

	void SetPixelSizeLimits(LOG_UNIT const fMin, LOG_UNIT const fMax)
	{
		m_pixelSizeMin = fMin;
		m_pixelSizeMax = fMax;
		ClipToMinMax(m_pixelSize, fMin, fMax);
	}
	
	void SetOffset     (fPixel const o) { m_fPixOffset = o; }
	void SetZoomFactor (float  const f) { m_zoomFactor = f; };

private:

	fPixel   m_fPixOffset;
	LOG_UNIT m_pixelSize;
	LOG_UNIT m_pixelSizeMin {   1.0f };
	LOG_UNIT m_pixelSizeMax { 100.0f };
	float    m_scaleFactor  {   1.0f };
	float    m_zoomFactor   {   1.1f };
};
