// SimulationControl.ixx
//
// NNetWindows

export module NNetWin32:SimulationControl;

import ObserverInterface;
import StatusBar;
import WinBasics;
import :Compute;

export class SimulationControl: public ObserverInterface
{
public:

	void Initialize(StatusBar * const, Compute * const);

	~SimulationControl() override = default;

	void Notify(bool const) final;

private:
	Compute * m_pCompute   { nullptr };
	HWND      m_hwndRunStop;
	HWND      m_hwndScan;
};
