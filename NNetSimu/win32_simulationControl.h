// win32_simulationControl.h
//
// NNetSimu

#pragma once

#include "observerInterface.h"
#include "win32_status.h"

class NNetWorkThreadInterface;

class SimulationControl: public ObserverInterface
{
public:

	SimulationControl( StatusBar * const, NNetWorkThreadInterface * const );

	virtual ~SimulationControl() {};

	virtual void Notify( bool const );

private:
	NNetWorkThreadInterface * m_pWorkThreadInterface;
	StatusBar               * m_pStatusBar;
};
