// TimeDisplay.h
//
// NNetSimu

#pragma once

#include "util.h"
#include "ObserverInterface.h"
#include "win32_refreshRate.h"
#include "win32_status.h"
#include "NNetReadBuffer.h"
#include "NNetModel.h"

using std::to_wstring;


class TimeDisplay : public ObserverInterface
{
public:
	TimeDisplay( StatusBar *, NNetReadBuffer *, int );

	virtual void Notify( bool const );

private:

	class TimeDisplayRefreshRate;

	TimeDisplayRefreshRate * m_pRefreshRate;
};

