// SetParameterCommand.h
//
// Commands

#pragma once

#include "tParameter.h"
#include "NNetParameters.h"
#include "NNetModelWriterInterface.h"
#include "Command.h"

class SetParameterCommand : public Command
{
public:
	SetParameterCommand
	( 
		Param      const * pParam,
		tParameter const   param, 
		float      const   fNewValue 
	)
	  : m_param( param ),
		m_fNewValue( fNewValue ),
		m_fOldValue( pParam->GetParameterValue( m_param ))
	{ }

	virtual void Do( NNetModelWriterInterface & model ) 
	{ 
		model.SetParam( m_param, m_fNewValue ); 
	}

	virtual void Undo( NNetModelWriterInterface & model ) 
	{ 
		model.SetParam( m_param, m_fOldValue ); 
	}

private:
	tParameter const m_param;
	float      const m_fOldValue;
	float      const m_fNewValue;
};
