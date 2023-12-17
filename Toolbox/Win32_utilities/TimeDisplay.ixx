// TimeDisplay.ixx
//
// Toolbox\Win32_utilities

module;

#include <memory>

export module TimeDisplay;

import ObserverInterface;
import StatusBar;

using std::unique_ptr;

export class TimeDisplay : public ObserverInterface
{
public:
	~TimeDisplay();

	void Initialize(StatusBar *, int);

	void Notify(bool const) final;

	class RefreshRate : public StatusBarRefreshRate
	{
	public:
		using StatusBarRefreshRate::StatusBarRefreshRate;
		void Trigger(bool const) final;
	};

private:

	unique_ptr<RefreshRate> m_upRefreshRate;
};
