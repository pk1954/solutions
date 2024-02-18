// SimulationControl.ixx
//
// NNetWindows

module;

#include <compare>
#include <Windows.h>

export module NNetWin32:SimulationControl;

import ObserverInterface;
import StatusBar;
import :Compute;

export class SimulationControl: public ObserverInterface
{
public:

	void Initialize(StatusBar * const, Compute * const);

	~SimulationControl() override = default;

	void Notify(bool const) final;

private:
	Compute   * m_pCompute   { nullptr };
	StatusBar * m_pStatusBar { nullptr };
	HWND        m_hwndSingleStep;
	HWND        m_hwndRunStop;
	HWND        m_hwndScan;
};
