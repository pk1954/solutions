// SimulationControl.ixx
//
// NNetWindows

module;

#include <compare>

export module NNetWin32:SimulationControl;

import ObserverInterface;
import :ComputeThread;
import :StatusBar;

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
