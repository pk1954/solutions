// SpeedDisplay.h
//
// NNetSimu

#pragma once

#include "ObserverInterface.h"
#include "NNetReadBuffer.h"

using std::to_wstring;

class StatusBar;
class SlowMotionRatio;

class SpeedDisplay : public ObserverInterface
{
public:
	SpeedDisplay( StatusBar *, SlowMotionRatio *, int );

	~SpeedDisplay();

	virtual void Notify( bool const );

private:

	class RefreshRate;

	RefreshRate * m_pRefreshRate;
};

