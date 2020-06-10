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
	void Start( StatusBar *, NNetModelReaderInterface const *, int );

	void Stop();

	virtual void Notify( bool const );

private:

	class RefreshRate;

	RefreshRate * m_pRefreshRate { nullptr };
};

