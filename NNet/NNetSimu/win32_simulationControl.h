// win32_simulationControl.h
//
// NNetSimu

#pragma once

#include "observerInterface.h"
#include "win32_status.h"

class WorkThreadInterface;

class SimulationControl: public ObserverInterface
{
public:

	SimulationControl( StatusBar * const, WorkThreadInterface * const );

	virtual ~SimulationControl() {};

	virtual void Notify( bool const );

private:
	WorkThreadInterface * m_pWorkThreadInterface;
	StatusBar           * m_pStatusBar;
};
