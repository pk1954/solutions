// Measurementhh
//
// NNetWindows

#pragma once

#include "PixelTypes.h"

class D2D_driver;

struct IDWriteTextFormat;

class Measurement
{
public:	

	void Initialize(D2D_driver * const);
	void SetClientRectSize(PIXEL const, PIXEL const);
	void DisplayDynamicScale(fMicroSecs const) const;
	void MoveSelection(fPixel const);
	bool Select(fPixel const);
	void ResetLimits();

	void SetActive(bool const bState) { m_bActive = bState; }
	bool IsActive()                    { return m_bActive; }
	bool TrackingActive()              { return m_bSelectedLeft || m_bSelectedRight; }

	bool IsClose2LeftLimit (fPixel const) const;
	bool IsClose2RightLimit(fPixel const) const;

private:
	D2D_driver        * m_pGraphics   { nullptr }; 
	IDWriteTextFormat * m_pTextFormat { nullptr };

	fPixel m_fPixClientWidth { fPixel::NULL_VAL() };
	fPixel m_fPixClientHeight{ fPixel::NULL_VAL() };

	fPixel m_fPixLeftLimit   { fPixel::NULL_VAL() };
	fPixel m_fPixRightLimit  { fPixel::NULL_VAL() };

	bool m_bSelectedLeft  { false };
	bool m_bSelectedRight { false };

	bool m_bActive      { true };
	bool m_bLimitsMoved { false };  // left or right limit moved by user?

	inline static fPixel const GRADIENT_WIDTH { 10._fPixel };

	void emphasizedLineLeft (fPixel const) const;
	void emphasizedLineRight(fPixel const) const;
	void verticalLine       (fPixel const) const;
	void measuringArea      () const;
	void textArea           (fMicroSecs const) const;
};
