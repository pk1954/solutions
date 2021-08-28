// SelectAllBeepersCommand.h
//
// Commands

#pragma once

#include "NNetModelWriterInterface.h"
#include "SelectionCommand.h"

class SelectAllBeepersCommand : public SelectionCommand
{
public:
	virtual void Do()
	{ 
		SelectionCommand::Do();
		m_pNMWI->SelectBeepers();
	}
};

