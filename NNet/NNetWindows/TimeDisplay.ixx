// TimeDisplay.ixx
//
// NNetWindows

module;

#pragma once

#include "win32_baseRefreshRate.h"

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

	class RefreshRate : public BaseRefreshRate
	{
	public:
		RefreshRate(StatusBar *, int);
		void Trigger() final;

	private:
		StatusBar * m_pStatusBar       { nullptr };
		int         m_iPartInStatusBar { -1 };
	};

private:

	unique_ptr<RefreshRate> m_upRefreshRate;
};
