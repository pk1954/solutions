// SimulationTime.h
//
// NNetModel

#pragma once

#include "MoreTypes.h"

class SimulationTime
{
public:
	static void Set(fMicroSecs const newVal = 0._MicroSecs) 
	{ 
		m_timeStamp = newVal; 
	}

	static fMicroSecs Get()                                       
	{ 
		return m_timeStamp; 
	}

	static void Tick(fMicroSecs const timeInc)  // called by compute thread!
	{ 
		m_timeStamp += timeInc; 
	}

private:

	inline static fMicroSecs m_timeStamp { 0._MicroSecs };
};