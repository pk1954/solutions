// DeleteSelectionCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModelWriterInterface.h"
#include "CommandFunctions.h"
#include "ShapeIdList.h"
#include "CommandStack.h"
#include "Command.h"

class DeleteSelectionCommand : public Command
{
public:

	virtual void Do(NNetModelWriterInterface & nmwi) 
	{ 
		if ( ! m_bInitialized )
		{
			init(nmwi);
			m_bInitialized = true;
		}
		m_cmdStack.DoAll();
	}

	virtual void Undo(NNetModelWriterInterface & nmwi) 
	{ 
		m_cmdStack.UndoAll();
	}

private:
	void init(NNetModelWriterInterface & nmwi) 
	{ 
		m_cmdStack.Initialize(&nmwi, nullptr);

		nmwi.GetUPShapes().Apply2AllSelected<Shape>
		(
			[&](Shape & shape) 
			{ 
				m_cmdStack.Push( move( MakeDeleteCommand(shape) ) );
			}
		); 
	}

	CommandStack m_cmdStack     {};
	ShapeIdList  m_idList       {};
	bool         m_bInitialized { false };
};