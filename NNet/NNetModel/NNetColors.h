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

	// internal alternative colors of objects when blinking
	inline static D2D1::ColorF const INT_BLINK_KEY_1 { 0.9f, 0.9f, 0.9f, 1.0f };
	inline static D2D1::ColorF const INT_BLINK_KEY_2 { 1.0f, 0.6f, 0.1f, 1.0f };

	void SetColSelectedAndSleep( D2D1::ColorF const );

	ObserverInterface * m_pObserver { nullptr };

	friend unsigned int __stdcall BlinkFunc( void * );

public:											 

	NNetColors( ObserverInterface * const );

	// colors for neuron/pipe boundary
	inline static D2D1::ColorF const EXT_NORMAL          { 0.0f, 0.5f, 1.0f, 1.0f };
	inline static D2D1::ColorF const EXT_MARKED          { 0.0f, 1.0f, 0.5f, 1.0f };
	inline static D2D1::ColorF const EXT_HIGHLIGHT       { 0.0f, 0.8f, 0.8f, 1.0f };
	inline static D2D1::ColorF const EXT_SUPER_HIGHLIGHT { 1.0f, 0.0f, 0.0f, 1.0f };

	// SELECTION_RECT is used to select rectangular area, transparent
	inline static D2D1::ColorF const SELECTION_RECT      { 1.0f, 0.0f, 0.0f, 0.5f };

	// color of selected shapes
	inline static D2D1::ColorF m_colSelected { INT_BLINK_KEY_1 };

	inline static MilliSecs BLINK_TIME { 200_MilliSecs };
};