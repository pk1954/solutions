// TimeDisplay.h
//
// NNetSimu

#pragma once

#include "NNetReadBuffer.h"
#include "ObserverInterface.h"

class StatusBar;

class TimeDisplay : public ObserverInterface
{
public:
	TimeDisplay( StatusBar *, NNetReadBuffer *, int );

	~TimeDisplay();

	virtual void Notify( bool const );

private:

	class RefreshRate;

	RefreshRate * m_pRefreshRate;
};

