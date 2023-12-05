// SimulationControl.ixx
//
// NNetWindows

module;

#include <compare>
#include <Windows.h>

export module NNetWin32:SimulationControl;

import ObserverInterface;
import StatusBar;
import :ComputeThread;

export class SimulationControl: public ObserverInterface
{
public:

	void Initialize(StatusBar * const, ComputeThread * const);

	~SimulationControl() override = default;

	void Notify(bool const) final;

private:
	ComputeThread * m_pComputeThread { nullptr };
	StatusBar     * m_pStatusBar     { nullptr };
	HWND            m_hwndSingleStep;
	HWND            m_hwndRunStop;
	HWND            m_hwndScan;
};
