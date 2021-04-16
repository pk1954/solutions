// DeleteSelectionCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModelWriterInterface.h"
#include "ShapeIdList.h"
#include "Command.h"

class DeleteSelectionCommand : public Command
{
public:
	DeleteSelectionCommand()
	{ }

	virtual void Do(NNetModelWriterInterface & nmwi) 
	{ 
		// detour with secondary list is neccessary! cannot delete shapes directly in Apply2All		

		ShapeIdList list(nmwi.GetUPShapes().GetAllSelected<Shape>()); // first construct list

		list.Apply2All                                                // then run through list 
		(                                                             // and delete shapes in model
			[&](ShapeId const & id) 
			{ 
				// deleteShape( id ); 
			}
		); 
	}

	virtual void Undo(NNetModelWriterInterface & nmwi) 
	{ 
	}

private:
	
};