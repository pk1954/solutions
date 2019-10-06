// SlowMotionDisplay.h
//
// NNetSimu

#pragma once

#include "ObserverInterface.h"
#include "NNetReadBuffer.h"

using std::to_wstring;

class StatusBar;
class SlowMotionRatio;

class SlowMotionDisplay : public ObserverInterface
{
public:
	SlowMotionDisplay( StatusBar *, SlowMotionRatio *, int );

	~SlowMotionDisplay();

	virtual void Notify( bool const );

private:

	class RefreshRate;

	RefreshRate * m_pRefreshRate;
};

