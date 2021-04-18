// DeleteSelectionCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModelWriterInterface.h"
#include "DeletePipeCommand.h"
#include "DisconnectBaseKnotCommand.h"
#include "DisconnectConnectorCommand.h"
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
		// detour with secondary list is neccessary! cannot delete shapes directly in Apply2All		

		//ShapeIdList list(nmwi.GetUPShapes().GetAllSelected<Shape>()); // first construct list

		nmwi.GetUPShapes().GetAllSelected<Shape>().Apply2All                                                // then run through list 
		(                                                             // and delete shapes in model
			[&](Shape & shape) 
			{ 
				ShapeId const id { shape.GetId() };
				unique_ptr<Command> pCmd;
				if (shape.IsPipe()) 
				{
					pCmd = make_unique<DeletePipeCommand>(id);
				}
				else if (shape.IsConnector()) 
				{
					pCmd = make_unique<DisconnectConnectorCommand>(id, true);
				}
				else 
				{
					pCmd = make_unique<DisconnectBaseKnotCommand>(id, true);
				}
				m_cmdStack.Push( move( pCmd ) );
			}
		); 
	}

	//void init(NNetModelWriterInterface & nmwi) 
	//{ 
	//	m_cmdStack.Initialize(&nmwi, nullptr);
	//	// detour with secondary list is neccessary! cannot delete shapes directly in Apply2All		

	//	ShapeIdList list(nmwi.GetUPShapes().GetAllSelected<Shape>()); // first construct list

	//	list.Apply2All                                                // then run through list 
	//	(                                                             // and delete shapes in model
	//		[&](ShapeId const & id) 
	//		{ 
	//			if ( Shape * pShape { nmwi.GetShape(id) } )
	//			{
	//				unique_ptr<Command> pCmd;
	//				if (pShape->IsPipe()) 
	//				{
	//					pCmd = make_unique<DeletePipeCommand>(id);
	//				}
	//				else if (pShape->IsConnector()) 
	//				{
	//					pCmd = make_unique<DisconnectConnectorCommand>(id, true);
	//				}
	//				else 
	//				{
	//					pCmd = make_unique<DisconnectBaseKnotCommand>(id, true);
	//				}
	//				m_cmdStack.Push( move( pCmd ) );
	//			}
	//		}
	//	); 
	//}

	CommandStack m_cmdStack {};
	ShapeIdList  m_idList   {};
	bool         m_bInitialized { false };
};