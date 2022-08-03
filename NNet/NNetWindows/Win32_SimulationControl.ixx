// Win32_SimulationControl.ixx
//
// NNetSimu

export module SimulationControl;

import ObserverInterface;
import ComputeThread;
import StatusBar;

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
