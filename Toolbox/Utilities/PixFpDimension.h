// PixFpDimension.h : 
//
// Utilities

#pragma once

#include "MoreTypes.h"
#include "PixelTypes.h"

import Util;
import Observable;

template <typename LOG_UNIT>
class PixFpDimension : public Observable
{
public:

	void Reset()
	{
		m_fPixOffset   = 0.0_fPixel;
		m_logPixelSize = LOG_UNIT(1.0f);
		NotifyAll(false);
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
		return Transform2fPixelSize(np) - m_fPixOffset;
	}

	LOG_UNIT Transform2logUnitPos(fPixel const fPixel) const
	{ 
		return Transform2logUnitSize(fPixel + m_fPixOffset);
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

	void Move(fPixel   const fPixDelta) { SetOffset(m_fPixOffset - fPixDelta); }
	void Move(PIXEL    const pixDelta ) { Move(::Convert2fPixel(pixDelta)); }
	void Move(LOG_UNIT const umDelta  ) { Move(Transform2fPixelSize(umDelta)); }

	bool ZoomDir(bool const bDirection, LOG_UNIT const logCenter, bool const bNotify = true) 
	{
		return zoomFactor(calcFactor(bDirection), Transform2fPixelPos(logCenter), logCenter, bNotify);
	}

	bool ZoomDir(bool const bDirection, fPixel const fPixCenter, bool const bNotify = true) 
	{
		return ZoomFactor(calcFactor(bDirection), fPixCenter, bNotify);
	}

	bool ZoomFactor(float const fFactor, fPixel const fPixCenter, bool const bNotify = true) 
	{
		return zoomFactor(fFactor, fPixCenter, Transform2logUnitPos(fPixCenter), bNotify);
	}

	bool zoomFactor
	(
		float    const fFactor, 
		fPixel   const fPixCenter, 
		LOG_UNIT const logCenter, 
		bool     const bNotify = true
	) 
	{
		LOG_UNIT logNewSize { m_logPixelSize * fFactor };
		if (IsValidPixelSize(logNewSize) )
		{
			m_logPixelSize = logNewSize;                                   // now zoom
			m_fPixOffset   = Transform2fPixelSize(logCenter) - fPixCenter; // and fix offset
			if (bNotify)
				NotifyAll(true);
			return true;
		}
		return false;
	}

	PixFpDimension operator+= (PixFpDimension const a) 
	{ 
		m_fPixOffset   += a.m_fPixOffset;
		m_logPixelSize += a.m_logPixelSize; 
		NotifyAll(true);
		return * this; 
	}

	PixFpDimension operator-= (PixFpDimension const a) 
	{ 
		m_fPixOffset   -= a.m_fPixOffset;
		m_logPixelSize -= a.m_logPixelSize; 
		NotifyAll(true);
		return * this; 
	}

	PixFpDimension operator*= (float const factor) 
	{ 
		m_fPixOffset   *= factor;
		m_logPixelSize *= factor; 
		NotifyAll(true);
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

	void SetPixelSizeLimits(LOG_UNIT const fMin, LOG_UNIT const fMax)
	{
		m_pixelSizeMin = fMin;
		m_pixelSizeMax = fMax;
		SetPixelSize(ClipToMinMax(m_logPixelSize, fMin, fMax));
	}
	
	void SetZoomFactor(float const f) 
	{ 
		m_fZoomFactor = f; 
	};

	void SetPixelSize(LOG_UNIT const s, bool const bNotify = true) 
	{ 
		m_logPixelSize = s; 
		if (bNotify)
			NotifyAll(true);
	}

	void SetOffset(fPixel const o, bool const bNotify = true) 
	{
		m_fPixOffset = o; 
		if (bNotify)
			NotifyAll(true);
	}

	float GetZoomFactor() const
	{ 
		return m_fZoomFactor; 
	};

private:

	fPixel   m_fPixOffset   { 0.0_fPixel };
	LOG_UNIT m_logPixelSize {       1.0f };
	LOG_UNIT m_pixelSizeMin {       1.0f };
	LOG_UNIT m_pixelSizeMax {     100.0f };
	float    m_fZoomFactor  {       1.1f };

	float calcFactor(bool const bDirection) const
	{
		return bDirection ? 1.0f / m_fZoomFactor : m_fZoomFactor;
	}
};
