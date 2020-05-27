// TimeDisplay.h
//
// NNetSimu

#pragma once

#include "ObserverInterface.h"

class StatusBar;
class NNetModelReaderInterface;

class TimeDisplay : public ObserverInterface
{
public:
	TimeDisplay( StatusBar *, NNetModelReaderInterface const *, int );

	~TimeDisplay();

	virtual void Notify( bool const );

private:

	class RefreshRate;

	RefreshRate * m_pRefreshRate;
};

