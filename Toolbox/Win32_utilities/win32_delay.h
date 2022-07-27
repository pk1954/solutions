// win32_delay.h 
//
// win32_utilities

#pragma once

#include <synchapi.h>
#include <chrono>

import Observable;

using std::chrono::milliseconds;

milliseconds const DEFAULT_DELAY(50);  
milliseconds const MAX_DELAY  (2048);  

class Delay: public Observable
{
public:

	milliseconds GetDelay() const
	{
		return m_msGenerationDelay;
	};

	bool IsMaxSpeed() const
	{
		return m_msGenerationDelay == milliseconds::zero();
	};

	void SetDelay(milliseconds delay)
	{
		m_msGenerationDelay = delay;
		NotifyAll(false);
	};

	void SleepDelay() const
	{
		if (m_msGenerationDelay > milliseconds::zero())
			Sleep(Cast2UnsignedLong(m_msGenerationDelay.count()));
	};

private:

	milliseconds m_msGenerationDelay { DEFAULT_DELAY };
};
