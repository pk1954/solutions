// SlowMotionDisplay.h
//
// NNetSimu

#pragma once

#include "observerInterface.h"

class StatusBar;
class SlowMotionRatio;

class SlowMotionDisplay : public ObserverInterface
{
public:
	void Initialize(StatusBar *, SlowMotionRatio *, int);

    void Notify(bool const) final;

private:

	class RefreshRate;

	wstring           m_wstring          { };
	int               m_iPartInStatusBar { -1 };
	StatusBar       * m_pStatusBar       { nullptr };
	SlowMotionRatio * m_pSlowMotionRatio { nullptr };
};
