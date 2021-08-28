// SelectAllCommand.h
//
// Commands

#pragma once

#include "BoolOp.h"
#include "NNetModelWriterInterface.h"
#include "SelectionCommand.h"

class SelectAllCommand : public SelectionCommand
{
public:
	SelectAllCommand(bool const bOn)
	  :	m_bOn(bOn)
	{ }

	virtual void Do() 
	{ 
		SelectionCommand::Do();
		m_pNMWI->GetUPNobs().SelectAllNobs(m_bOn); 
	}

private:
	bool m_bOn;
};
