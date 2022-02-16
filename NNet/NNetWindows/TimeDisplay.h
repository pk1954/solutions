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
	void Start(StatusBar *, NNetModelReaderInterface const *, int);

	void Stop();

	void Notify(bool const) final;

private:

	class RefreshRate;

	RefreshRate * m_pRefreshRate { nullptr };
};

