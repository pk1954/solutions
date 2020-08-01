// SelectAllBeepersCommand.h
//
// Commands

#pragma once

#include "NNetModel.h"
#include "SelectionCommand.h"

class SelectAllBeepersCommand : public SelectionCommand
{
public:
	SelectAllBeepersCommand( NNetModel * const pModel )
		:	SelectionCommand( pModel)
	{ }

	virtual void Do( NNetModel * const pModel ) 
	{ 
		pModel->SelectBeepers();
	}
};

