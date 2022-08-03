// Win32_SimulationControl.ixx
//
// NNetSimu

module;

#include "win32_status.h"
#include "ComputeThread.h"

export module SimulationControl;

import ObserverInterface;

export class SimulationControl: public ObserverInterface
{
public:

	void Initialize(StatusBar * const, ComputeThread * const);

	~SimulationControl() override = default;

	void Notify(bool const) final;

private:
	ComputeThread * m_pComputeThread { nullptr };
	StatusBar     * m_pStatusBar     { nullptr };
};
