// TimeDisplay.ixx
//
// NNetWindows

module;

#include <memory>

export module NNetWin32:TimeDisplay;

import ObserverInterface;
import BaseRefreshRate;
import StatusBar;

using std::unique_ptr;

export class TimeDisplay : public ObserverInterface
{
public:
	~TimeDisplay();

	void Initialize(StatusBar *, int);

	void Notify(bool const) final;

	class RefreshRate : public BaseRefreshRate
	{
	public:
		RefreshRate(StatusBar *, int);
		void Trigger(bool const = false) final;

	private:
		StatusBar * m_pStatusBar       { nullptr };
		int         m_iPartInStatusBar { -1 };
	};

private:

	unique_ptr<RefreshRate> m_upRefreshRate;
};
