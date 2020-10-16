// SelectAllBeepersCommand.h
//
// Commands

#pragma once

#include "NNetModelWriterInterface.h"
#include "SelectionCommand.h"

class SelectAllBeepersCommand : public SelectionCommand
{
public:
	SelectAllBeepersCommand( NNetModelWriterInterface * const pModel )
		:	SelectionCommand( pModel)
	{ }

	virtual void Do( NNetModelWriterInterface * const pModel )
	{ 
		pModel->SelectBeepers();
	}
};

