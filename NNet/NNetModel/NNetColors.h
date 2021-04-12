// NNetColors.h
//
// NNetModel

#pragma once

#include "d2d1helper.h"
												  
class NNetColors								  
{												 
public:											 

	// SELECTION_RECT is used to select rectangular area, transparent
	inline static D2D1::ColorF const SELECTION_RECT { 1.0f, 0.0f, 0.0f, 0.2f };

	// POSITION_RECT is used in mini window to visualize position of main window area, transparent
	inline static D2D1::ColorF const POSITION_RECT  { 0.9f, 0.3f, 0.2f, 0.4f };

	// color of beacon 
	inline static D2D1::ColorF const COL_BEACON     { 0.0f, 0.0f, 1.0f, 0.4f };

	// colors EEG-Monitor
	inline static D2D1::ColorF const COL_DIAMOND    { 0.0f, 1.0f, 0.0f, 1.0f };

	// colors for neuron/pipe boundary
	inline static D2D1::ColorF const EXT_NORMAL       { 0.0f, 0.5f, 1.0f, 1.0f };
	inline static D2D1::ColorF const INT_NORMAL       { D2D1::ColorF::Black };
	inline static D2D1::ColorF const EXT_HIGHLIGHTED  { 0.4f, 0.8f, 0.8f, 1.0f  };
	inline static D2D1::ColorF const INT_HIGHLIGHTED  { INT_NORMAL };
	inline static D2D1::ColorF const EXT_SELECTED     { EXT_NORMAL };
	inline static D2D1::ColorF const INT_SELECTED     { 1.0f, 0.6f, 0.1f, 1.0f };
	inline static D2D1::ColorF const EXT_TARGET       { 0.0f, 0.6f, 0.0f, 1.0f };  // dark green
	inline static D2D1::ColorF const INT_TARGET_FIT   { 0.0f, 1.0f, 0.0f, 1.0f };  // green
	inline static D2D1::ColorF const INT_TARGET_NOFIT { 1.0f, 0.0f, 0.0f, 1.0f };  // red
	inline static D2D1::ColorF const EXT_CONNECTOR    { 0.75f, 0.0f, 0.75f, 1.0f };

	// color used when neurons trigger
	inline static D2D1::ColorF const INT_TRIGGER    { 1.0f, 1.0f, 0.0f, 1.0f };

	// construct color with given alpha channel
	static D2D1::ColorF const SetAlpha( D2D1::ColorF col, float const alpha )
	{
		col.a = alpha;
		return col;
	}
};