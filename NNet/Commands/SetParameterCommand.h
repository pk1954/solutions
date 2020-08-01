// SetParameterCommand.h
//
// Commands

#pragma once

#include "tParameter.h"
#include "NNetModel.h"
#include "Command.h"

class SetParameterCommand : public Command
{
public:
	SetParameterCommand( tParameter const param, float const fNewValue )
	  : m_param( param ),
		m_fValue( fNewValue )
	{ }

	virtual void Do( NNetModel * const pModel ) 
	{ 
		m_fValue = pModel->SetParameter( m_param, m_fValue ); 
	}

private:
	tParameter const m_param;
	float            m_fValue;
};
