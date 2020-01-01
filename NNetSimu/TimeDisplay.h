// TimeDisplay.h
//
// NNetSimu

#pragma once

#include "ObserverInterface.h"

class StatusBar;
class NNetModel;

class TimeDisplay : public ObserverInterface
{
public:
	TimeDisplay( StatusBar *, NNetModel const *, int );

	~TimeDisplay();

	virtual void Notify( bool const );

private:

	class RefreshRate;

	RefreshRate * m_pRefreshRate;
};

