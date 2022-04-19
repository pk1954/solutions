// Stimulus.h
//
// NNetModel

#pragma once

#include "MoreTypes.h"
#include "observable.h"

class Stimulus : public Observable  // dynamic data of stimulus
{
public:

	void TriggerStimulus()
	{
		m_usSinceLastStimulus = 0._MicroSecs;
		m_bTriggerActive = true;
		NotifyAll(false);
	}

	void Tick(fMicroSecs const tIncr)
	{
		m_usSinceLastStimulus += tIncr;
		NotifyAll(false);
	}
	void StopTrigger() { m_bTriggerActive = false; }

	bool       IsTriggerActive() const { return m_bTriggerActive; }
	fMicroSecs TimeTilTrigger () const { return m_usSinceLastStimulus; }

private:
	bool       m_bTriggerActive      { false };
	fMicroSecs m_usSinceLastStimulus { 0._MicroSecs };
};

