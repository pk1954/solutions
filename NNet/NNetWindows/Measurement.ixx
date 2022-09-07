// Measurement.ixx
//
// NNetWindows

module;

#include <dwrite.h>

export module Measurement;

import Types;
import Direct2D;

export class Measurement
{
public:	

	void Initialize(D2D_driver * const);
	void SetClientRectSize(PIXEL const, PIXEL const);
	void DisplayDynamicScale(fMicroSecs const) const;
	void MoveSelection(fPixel const);
	bool Select(fPixel const);
	void ResetLimits();

	void SetActive(bool const bState) { m_bActive = bState; }

	bool IsActive()       const { return m_bActive; }
	bool TrackingActive() const { return m_bLeftActive || m_bRightActive; }

	bool IsClose2LeftLimit (fPixel const) const;
	bool IsClose2RightLimit(fPixel const) const;

private:
	D2D_driver        * m_pGraphics   { nullptr }; 
	IDWriteTextFormat * m_pTextFormat { nullptr };

	fPixel m_fPixClientWidth { fPixel::NULL_VAL() };
	fPixel m_fPixClientHeight{ fPixel::NULL_VAL() };

	fPixel m_fPixSelLeft   { fPixel::NULL_VAL() };
	fPixel m_fPixSelRight  { fPixel::NULL_VAL() };

	bool m_bLeftActive  { false };
	bool m_bRightActive { false };
	bool m_bActive      { false };

	inline static fPixel const GRADIENT_WIDTH { 10._fPixel };

	void emphasizedLineLeft (fPixel const) const;
	void emphasizedLineRight(fPixel const) const;
	void verticalLine       (fPixel const) const;
	void measuringArea      () const;
	void textArea           (fMicroSecs const) const;
};
