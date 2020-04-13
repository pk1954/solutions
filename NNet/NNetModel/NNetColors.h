// NNetColors.h
//
// NNetModel

#pragma once

#include "d2d1helper.h"
#include "ObserverInterface.h"

class NNetColors
{
public:
	NNetColors( ObserverInterface * const );

	// normal color of neuron/pipe boundary
	static D2D1::ColorF const EXT_NORMAL;

	// color of highlighted ans super highlighted boundary
	static D2D1::ColorF const EXT_HIGHLIGHT;
	static D2D1::ColorF const EXT_SUPER_HIGHLIGHT;

	// low key colors are used to damp down objects so that emphasized objects have better contrast
	static D2D1::ColorF const EXT_LOW_KEY;
	static D2D1::ColorF const INT_LOW_KEY;

	// COLOR_SELECTION_RECT is used to select rectangular area, transparent
	static D2D1::ColorF const SELECTION_RECT;

	// color of selected shapes
	static D2D1::ColorF m_colSelected;

private:

	void SetColSelectedAndSleep( D2D1::ColorF const color )
	{
		m_colSelected = color;
		m_pObserver->Notify( true );
		Sleep( 500 );
	}

	ObserverInterface * m_pObserver { nullptr };

	friend unsigned int __stdcall BlinkFunc( void * );
};