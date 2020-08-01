// ResetTimerCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModel.h"
#include "Command.h"

class ResetTimerCommand : public Command
{
public:
	virtual void Do( NNetModel * const pModel ) 
	{ 
		m_simuTime = pModel->SetSimulationTime( m_simuTime); 
	}

private:
	fMicroSecs m_simuTime { 0._MicroSecs };
};
