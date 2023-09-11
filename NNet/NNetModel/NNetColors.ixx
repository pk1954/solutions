// NNetColors.ixx
//
// NNetModel

module;

#include "d2d1helper.h"

export module NNetModel:NNetColors;

export using D2D1::ColorF;

import Color;

export class NNetColors						  
{												 
public:											 

	// SCAN_AREA_RECT 
	inline static Color const SCAN_AREA_RECT    { 0.0f, 0.0f, 1.0f, 0.2f };
	inline static Color const SCAN_AREA_HANDLE  { 0.0f, 0.0f, 1.0f, 0.5f };

	// POSITION_RECT is used in mini window to visualize position of main window area, transparent
	inline static Color const POSITION_RECT     { 0.9f, 0.3f, 0.2f, 0.4f };

	// colors for EEG sensors
	inline static Color const EEG_SENSOR_1      { 0.0f, 0.5f, 0.0f, 0.8f };
	inline static Color const EEG_SENSOR_2      { 1.0f, 1.0f, 0.0f, 0.4f };
	inline static Color const EEG_SENSOR_HIGH   { 1.0f, 0.5f, 0.0f, 0.8f };

	inline static Color const MICRO_SENSOR      { 0.6f, 0.6f, 0.0f, 0.8f };
	inline static Color const MICRO_SENSOR_HIGH { 0.6f, 0.3f, 0.0f, 0.8f };

	// colors for neuron/pipe boundary
	inline static Color const EXT_NORMAL       { 0.0f, 0.5f, 1.0f, 1.0f };
	inline static Color const INT_NORMAL       { ColorF::Black };
	inline static Color const EXT_HIGHLIGHTED  { 0.4f, 0.8f, 0.8f, 1.0f  };
	inline static Color const INT_HIGHLIGHTED  { INT_NORMAL };
	inline static Color const EXT_SELECTED     { EXT_NORMAL };
	inline static Color const INT_SELECTED     { 1.0f, 0.6f, 0.1f, 1.0f };
	inline static Color const EXT_TARGET       { 0.0f, 0.6f, 0.0f, 1.0f };  // dark green
	inline static Color const INT_TARGET_FIT   { 0.0f, 1.0f, 0.0f, 1.0f };  // green
	inline static Color const INT_TARGET_NOFIT { 1.0f, 0.0f, 0.0f, 1.0f };  // red
	inline static Color const EXT_TARGET_FIT   { INT_TARGET_FIT };
	inline static Color const EXT_TARGET_NOFIT { INT_TARGET_NOFIT }; 
	inline static Color const EXT_EMPHASIZED   { 0.1f, 0.9f, 0.1f, 1.0f };
	inline static Color const INT_EMPHASIZED   { 0.2f, 0.4f, 0.2f, 1.0f };
	inline static Color const INT_EMP_SELECTED { INT_SELECTED };
	inline static Color const INT_BLOCKED      { 1.0f, 0.0f, 1.0f, 1.0f };

	// color used when neurons trigger
	inline static Color const INT_TRIGGER      { 1.0f, 1.0f, 0.0f, 1.0f };

	// construct color with given alpha channel
	Color SetAlpha(Color col, float const alpha)
	{
		col.a = alpha;
		return col;
	}
};