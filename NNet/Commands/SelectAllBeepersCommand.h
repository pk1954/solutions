// SelectAllBeepersCommand.h
//
// Commands

#pragma once

#include "NNetModelWriterInterface.h"
#include "SelectionCommand.h"

class SelectAllBeepersCommand : public SelectionCommand
{
public:
	SelectAllBeepersCommand( NNetModelWriterInterface & model )
		:	SelectionCommand( model)
	{ }

	virtual void Do( NNetModelWriterInterface & model )
	{ 
		model.SelectBeepers();
	}
};

