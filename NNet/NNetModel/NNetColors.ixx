// NNetColors.ixx
//
// NNetModel

module;

#include "d2d1helper.h"

export module NNetModel:NNetColors;

export using D2D1::ColorF;

export class NNetColors						  
{												 
public:											 

	// SELECTION_RECT is used to select rectangular area, transparent
	inline static ColorF const SELECTION_RECT   { 1.0f, 0.0f, 0.0f, 0.2f };

	// POSITION_RECT is used in mini window to visualize position of main window area, transparent
	inline static ColorF const POSITION_RECT    { 0.9f, 0.3f, 0.2f, 0.4f };

	// colors EEG Monitor
	inline static ColorF const COL_DIAMOND       { 0.0f, 1.0f, 0.0f, 1.0f };
	inline static ColorF const COL_STIMULUS_LINE { 0.5f, 0.1f, 0.1f, 1.0f };
	inline static ColorF const COL_TRACK_EVEN    { 0.9f, 0.9f, 0.9f, 1.0f };
	inline static ColorF const COL_TRACK_EVEN_H  { 0.9f, 0.9f, 1.0f, 1.0f };
	inline static ColorF const COL_TRACK_ODD     { 0.8f, 0.8f, 0.8f, 1.0f };
	inline static ColorF const COL_TRACK_ODD_H   { 0.8f, 0.8f, 0.9f, 1.0f };
	inline static ColorF const COL_WARNING       { 0.8f, 0.0f, 0.0f, 0.3f };
	inline static ColorF const EEG_SIGNAL_HIGH   { 1.0f, 0.5f, 0.0f, 1.0f };

	// colors for EEG sensors
	inline static ColorF const EEG_SENSOR_1      { 0.0f, 0.5f, 0.0f, 0.8f };
	inline static ColorF const EEG_SENSOR_2      { 1.0f, 1.0f, 0.0f, 0.4f };
	inline static ColorF const EEG_SENSOR_HIGH   { 1.0f, 0.5f, 0.0f, 0.8f };

	inline static ColorF const MICRO_SENSOR      { 0.7f, 0.7f, 0.0f, 0.8f };
	inline static ColorF const MICRO_SENSOR_HIGH { 0.7f, 0.5f, 0.0f, 0.8f };

	// colors for neuron/pipe boundary
	inline static ColorF const EXT_NORMAL       { 0.0f, 0.5f, 1.0f, 1.0f };
	inline static ColorF const INT_NORMAL       { ColorF::Black };
	inline static ColorF const EXT_HIGHLIGHTED  { 0.4f, 0.8f, 0.8f, 1.0f  };
	inline static ColorF const INT_HIGHLIGHTED  { INT_NORMAL };
	inline static ColorF const EXT_SELECTED     { EXT_NORMAL };
	inline static ColorF const INT_SELECTED     { 1.0f, 0.6f, 0.1f, 1.0f };
	inline static ColorF const EXT_TARGET       { 0.0f, 0.6f, 0.0f, 1.0f };  // dark green
	inline static ColorF const INT_TARGET_FIT   { 0.0f, 1.0f, 0.0f, 1.0f };  // green
	inline static ColorF const INT_TARGET_NOFIT { 1.0f, 0.0f, 0.0f, 1.0f };  // red
	inline static ColorF const EXT_TARGET_FIT   { INT_TARGET_FIT };
	inline static ColorF const EXT_TARGET_NOFIT { INT_TARGET_NOFIT }; 
	inline static ColorF const EXT_EMPHASIZED   { 0.1f, 0.9f, 0.1f, 1.0f };
	inline static ColorF const INT_EMPHASIZED   { 0.2f, 0.4f, 0.2f, 1.0f };
	inline static ColorF const INT_EMP_SELECTED { INT_SELECTED };
	inline static ColorF const INT_BLOCKED      { 1.0f, 0.0f, 1.0f, 1.0f };

	// color used when neurons trigger
	inline static ColorF const INT_TRIGGER    { 1.0f, 1.0f, 0.0f, 1.0f };

	// construct color with given alpha channel
	ColorF SetAlpha(ColorF col, float const alpha)
	{
		col.a = alpha;
		return col;
	}
};