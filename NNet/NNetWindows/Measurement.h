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

	void Initialize( D2D_driver * const );
	void SetClientRectSize( PIXEL const, PIXEL const );
	void DisplayDynamicScale( fMicroSecs const ) const;
	void MoveSelection( fPIXEL const );
	bool Select( fPIXEL const );
	void ResetLimits( );

	void SetActive( bool const bState ) { m_bActive = bState; }
	bool IsActive( )                    { return m_bActive; }
	bool TrackingActive( )              { return m_bSelectedLeft || m_bSelectedRight; }

	bool IsClose2LeftLimit ( fPIXEL const ) const;
	bool IsClose2RightLimit( fPIXEL const ) const;

private:
	D2D_driver        * m_pGraphics   { nullptr }; 
	IDWriteTextFormat * m_pTextFormat { nullptr };

	fPIXEL m_fPixClientWidth { fPIXEL::NULL_VAL() };
	fPIXEL m_fPixClientHeight{ fPIXEL::NULL_VAL() };

	fPIXEL m_fPixLeftLimit   { fPIXEL::NULL_VAL() };
	fPIXEL m_fPixRightLimit  { fPIXEL::NULL_VAL() };

	bool m_bSelectedLeft  { false };
	bool m_bSelectedRight { false };

	bool m_bActive      { true };
	bool m_bLimitsMoved { false };  // left or right limit moved by user?

	inline static fPIXEL const GRADIENT_WIDTH { 10._fPIXEL };

	void emphasizedLineLeft ( fPIXEL const ) const;
	void emphasizedLineRight( fPIXEL const ) const;
	void verticalLine       ( fPIXEL const ) const;
	void measuringArea      ( ) const;
	void textArea           ( fMicroSecs const ) const;
};
