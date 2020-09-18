// AddModelCommand.h
//
// Commands

#pragma once

#include "SelectionCommand.h"

class AddModelCommand : public SelectionCommand
{
public:

	AddModelCommand( NNetModelWriterInterface * const pModel )
		:	SelectionCommand( pModel)
	{ 

	}

	virtual void Do( NNetModelWriterInterface * const pModel ) 
	{ 

	}

private:
	ShapeList m_copies;
};

