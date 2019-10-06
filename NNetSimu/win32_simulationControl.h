// win32_simulationControl.h
//
// NNetSimu

#pragma once

#include "win32_status.h"

class SimulationControl
{
public:

	static void Add( StatusBar * const );
	static void Adjust( BOOL const,	WorkThreadInterface * const );

private:
	static StatusBar * m_pStatusBar;
};
