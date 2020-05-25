// win32_simulationControl.h
//
// NNetSimu

#pragma once

#include "observerInterface.h"
#include "win32_status.h"

class ComputeThread;

class SimulationControl: public ObserverInterface
{
public:

	SimulationControl( StatusBar * const, ComputeThread * const );

	virtual ~SimulationControl() {};

	virtual void Notify( bool const );

private:
	ComputeThread * m_pComputeThread;
	StatusBar     * m_pStatusBar;
};
