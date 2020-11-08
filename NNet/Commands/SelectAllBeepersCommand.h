// SelectAllBeepersCommand.h
//
// Commands

#pragma once

#include "NNetModelWriterInterface.h"
#include "SelectionCommand.h"

class SelectAllBeepersCommand : public SelectionCommand
{
public:
	virtual void Do( NNetModelWriterInterface & nmwi )
	{ 
		SelectionCommand::Do( nmwi );
		nmwi.SelectBeepers();
	}
};

