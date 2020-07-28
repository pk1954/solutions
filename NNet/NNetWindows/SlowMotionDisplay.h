// SlowMotionDisplay.h
//
// NNetSimu

#pragma once

#include "ObserverInterface.h"

class StatusBar;
class SlowMotionRatio;

class SlowMotionDisplay : public ObserverInterface
{
public:
	void Initialize( StatusBar *, SlowMotionRatio *, int );

	virtual void Notify( bool const );

private:

	class RefreshRate;

	wstring           m_wstring;
	StatusBar       * m_pStatusBar;
	int               m_iPartInStatusBar;
	SlowMotionRatio * m_pSlowMotionRatio;
};
