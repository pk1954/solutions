// NNetColors.h
//
// NNetModel

#pragma once

#include "MoreTypes.h"
#include "d2d1helper.h"
#include "Observable.h"                   
												  
class NNetColors								  
{												 
public:											 

	inline static D2D1::ColorF const COL_BLACK       { 0.0f, 0.0f, 0.0f, 1.0f };
	inline static D2D1::ColorF const COL_RED         { 1.0f, 0.0f, 0.0f, 1.0f };

	// colors for neuron/pipe boundary
	inline static D2D1::ColorF const EXT_NORMAL      { 0.0f, 0.5f, 1.0f, 1.0f };
	inline static D2D1::ColorF const INT_NORMAL      { COL_BLACK };
	inline static D2D1::ColorF const EXT_SELECTED    { 0.0f, 0.8f, 0.8f, 1.0f };
	inline static D2D1::ColorF const INT_SELECTED    { 1.0f, 0.6f, 0.1f, 1.0f };
	inline static D2D1::ColorF const EXT_TARGET      { COL_RED };
	inline static D2D1::ColorF const INT_TARGET      { COL_RED };

	// color used when neurons trigger
	inline static D2D1::ColorF const INT_TRIGGER     { 1.0f, 1.0f, 0.0f, 1.0f };

	// SELECTION_RECT is used to select rectangular area, transparent
	inline static D2D1::ColorF const SELECTION_RECT { 1.0f, 0.0f, 0.0f, 0.2f };

	// POSITION_RECT is used in mini window to visualize position of main window area, transparent
	inline static D2D1::ColorF const POSITION_RECT  { 0.9f, 0.3f, 0.2f, 0.4f };

	// color of beacon 
	inline static D2D1::ColorF CONST COL_BEACON     { 0.0f, 0.0f, 1.0f, 0.4f };
};