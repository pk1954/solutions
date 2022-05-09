// PixFpDimension.h : 
//
// Utilities

#pragma once

#include "util.h"
#include "observable.h"
#include "MoreTypes.h"
#include "PixelTypes.h"

template <typename LOG_UNIT>
class PixFpDimension : public Observable
{
public:

	void Reset()
	{
		m_fPixOffset   = 0.0_fPixel;
		m_logPixelSize = LOG_UNIT(1.0f);
	}

	//////// transformations LOG_UNIT <---> fPixel ////////

	fPixel Transform2fPixelSize(LOG_UNIT const param) const
	{ 
		return fPixel(param / m_logPixelSize);
	}

	LOG_UNIT Transform2logUnitSize(fPixel const fPixel) const
	{ 
		return m_logPixelSize * fPixel.GetValue();
	}

	fPixel Transform2fPixelPos(LOG_UNIT const np) const
	{ 
		return Transform2fPixelSize(np) + m_fPixOffset;
	}

	LOG_UNIT Transform2logUnitPos(fPixel const fPixel) const
	{ 
		return Transform2logUnitSize(fPixel - m_fPixOffset);
	}

	//////// transformations LOG_UNIT <---> PIXEL  ////////

	LOG_UNIT Transform2logUnitSize(PIXEL const pix) const
	{ 
		return Transform2logUnitSize(::Convert2fPixel(pix));
	}

	//////// queries ////////

	LOG_UNIT GetPixelSize()   const { return m_logPixelSize; };
	fPixel   GetPixelOffset() const { return m_fPixOffset; }

	//////// manipulation functions ////////

	void Move(fPixel   const fPixDelta) { m_fPixOffset -= fPixDelta; }
	void Move(PIXEL    const pixDelta ) { Move(::Convert2fPixel(pixDelta)); }
	void Move(LOG_UNIT const umDelta  ) { Move(Transform2fPixelSize(umDelta)); }

	bool Zoom(bool const bDirection)
	{
		bool     bResult { false };
		LOG_UNIT logNewSize;
		if (bDirection)
		{
			logNewSize = m_logPixelSize / m_fZoomFactor;
			if (logNewSize >= m_pixelSizeMin)
				bResult = true;
		}
		else
		{
			logNewSize = m_logPixelSize * m_fZoomFactor;
			if (logNewSize <= m_pixelSizeMax)
				bResult = true;
		}
		if (bResult)
			m_logPixelSize = logNewSize;
		NotifyAll(false);
		return true;
	}

	void Zoom(float const factor)
	{
		*this *= factor;
	}

	void Center
	(
		LOG_UNIT const umCenter,   
		fPixel   const fPntPix  
	)
	{
		SetPixelOffset(Transform2fPixelSize(umCenter) - fPntPix);
	}

	PixFpDimension operator+= (PixFpDimension const a) 
	{ 
		m_fPixOffset   += a.m_fPixOffset;
		m_logPixelSize += a.m_logPixelSize; 
		NotifyAll(false);
		return * this; 
	}

	PixFpDimension operator-= (PixFpDimension const a) 
	{ 
		m_fPixOffset   -= a.m_fPixOffset;
		m_logPixelSize -= a.m_logPixelSize; 
		NotifyAll(false);
		return * this; 
	}

	PixFpDimension operator*= (float const factor) 
	{ 
		m_fPixOffset   *= factor;
		m_logPixelSize *= factor; 
		NotifyAll(false);
		return * this; 
	}

	friend PixFpDimension operator+ (PixFpDimension const a, PixFpDimension const b) 
	{ 
		PixFpDimension res { a }; 
		res += b; 
		return res; 
	};

	friend PixFpDimension operator- (PixFpDimension const a, PixFpDimension const b) 
	{ 
		PixFpDimension res { a }; 
		res -= b; 
		return res; 
	};

	friend PixFpDimension operator* (PixFpDimension const a, float const factor) 
	{ 
		PixFpDimension res { a }; 
		res *= factor; 
		return res; 
	};

	bool IsValidPixelSize(LOG_UNIT const size) const
	{
		return (m_pixelSizeMin <= size) && (size <= m_pixelSizeMax); 
	}

	void SetPixelSize(LOG_UNIT const s) 
	{ 
		m_logPixelSize = s; 
		NotifyAll(false);
	}

	void SetPixelSizeLimits(LOG_UNIT const fMin, LOG_UNIT const fMax)
	{
		m_pixelSizeMin = fMin;
		m_pixelSizeMax = fMax;
		ClipToMinMax(m_logPixelSize, fMin, fMax);
	}
	
	void SetZoomFactor(float const f) 
	{ 
		m_fZoomFactor = f; 
	};

	void SetOffset(fPixel const o) 
	{
		m_fPixOffset = o; 
		NotifyAll(false);
	}

private:

	fPixel   m_fPixOffset   { 0.0_fPixel };
	LOG_UNIT m_logPixelSize {       1.0f };
	LOG_UNIT m_pixelSizeMin {       1.0f };
	LOG_UNIT m_pixelSizeMax {     100.0f };
	float    m_fZoomFactor  {       1.1f };
};
