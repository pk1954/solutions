// TimeDisplay.h
//
// NNetSimu

#pragma once

#include "observerInterface.h"
#include "win32_baseRefreshRate.h"

class StatusBar;

using std::unique_ptr;

class TimeDisplay : public ObserverInterface
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
