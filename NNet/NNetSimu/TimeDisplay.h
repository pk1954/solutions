// TimeDisplay.h
//
// NNetSimu

#pragma once

#include "ObserverInterface.h"

class StatusBar;
class NNetModelInterface;

class TimeDisplay : public ObserverInterface
{
public:
	TimeDisplay( StatusBar *, NNetModelInterface const *, int );

	~TimeDisplay();

	virtual void Notify( bool const );

private:

	class RefreshRate;

	RefreshRate * m_pRefreshRate;
};

