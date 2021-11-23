// SelectAllBeepersCommand.h
//
// Commands

#pragma once

#include "NNetModelWriterInterface.h"
#include "SelectionCommand.h"

class SelectAllBeepersCommand : public SelectionCommand
{
public:
	void Do() final
	{ 
		SelectionCommand::Do();
		m_pNMWI->SelectBeepers();
	}
};

