// NNetColors.h
//
// NNetModel

#pragma once

#include "MoreTypes.h"
#include "d2d1helper.h"
#include "ObserverInterface.h"                   
												  
class NNetColors								  
{												 
private:

	inline static ObserverInterface * m_pObserver { nullptr };
	inline static bool                m_bSwitch   { false };
	inline static UINT_PTR            m_TimerId   { 0 }; 

	friend VOID CALLBACK TimerProc( HWND, UINT, UINT_PTR, DWORD );

	inline static MilliSecs BLINK_TIME { 200_MilliSecs };

	// internal alternative colors of objects when blinking
	inline static D2D1::ColorF const INT_BLINK_KEY_1 { 0.9f, 0.9f, 0.9f, 1.0f };
	inline static D2D1::ColorF const INT_BLINK_KEY_2 { 1.0f, 0.6f, 0.1f, 1.0f };

public:											 

	NNetColors( ObserverInterface * const );
	~NNetColors( );

	// colors for neuron/pipe boundary
	inline static D2D1::ColorF const EXT_NORMAL          { 0.0f, 0.5f, 1.0f, 1.0f };
	inline static D2D1::ColorF const EXT_MARKED          { 0.0f, 1.0f, 0.5f, 1.0f };
	inline static D2D1::ColorF const EXT_HIGHLIGHT       { 0.0f, 0.8f, 0.8f, 1.0f };
	inline static D2D1::ColorF const EXT_SUPER_HIGHLIGHT { 1.0f, 0.0f, 0.0f, 1.0f };

	// color used when neurons trigger
	inline static D2D1::ColorF const INT_TRIGGER         { 255.0f, 255.0f, 0.0f, 1.0f };

	// SELECTION_RECT is used to select rectangular area, transparent
	inline static D2D1::ColorF const SELECTION_RECT      { 1.0f, 0.0f, 0.0f, 0.5f };

	// POSITION_RECT is used in mini window to visualize position of main window area, transparent
	inline static D2D1::ColorF const POSITION_RECT      { 0.8f, 0.8f, 0.3f, 0.7f };

	// color of selected shapes
	inline static D2D1::ColorF m_colSelected { INT_BLINK_KEY_1 };
};