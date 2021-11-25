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

	void Initialize(StatusBar * const, ComputeThread * const);

	~SimulationControl() override = default;

	void Notify(bool const) final;

private:
	ComputeThread * m_pComputeThread { nullptr };
	StatusBar     * m_pStatusBar     { nullptr };
};
