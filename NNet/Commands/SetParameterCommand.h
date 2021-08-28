// SetParameterCommand.h
//
// Commands

#pragma once

#include "ParameterType.h"
#include "NNetParameters.h"
#include "NNetModelWriterInterface.h"
#include "Command.h"

class SetParameterCommand : public Command
{
public:
	SetParameterCommand
	(
		Param            const & param,
		ParamType::Value const   parameter, 
		float            const   fNewValue 
	)
	  : m_parameter(parameter),
		m_fNewValue(fNewValue),
		m_fOldValue(param.GetParameterValue(parameter))
	{ }

	virtual void Do() 
	{ 
		m_pNMWI->SetParam(m_parameter, m_fNewValue); 
	}

	virtual void Undo() 
	{ 
		m_pNMWI->SetParam(m_parameter, m_fOldValue); 
	}

private:
	ParamType::Value const m_parameter;
	float            const m_fOldValue;
	float            const m_fNewValue;
};
