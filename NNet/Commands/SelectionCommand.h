// SelectionCommand.h
//
// Commands

#pragma once

#include "NNetModelWriterInterface.h"
#include "ShapePtrList.h"
#include "Command.h"

class SelectionCommand : public Command
{
public:

	virtual void Do( NNetModelWriterInterface & nmwi ) 
	{
		if ( ! m_bInitialized )	
		{ 
			nmwi.GetUPShapes().Apply2AllSelected<Shape>
			( 
				[&](Shape &s) { m_selectedShapes.Add(&s); } 
			);
			m_bInitialized = true;
		}
	}

	virtual void Undo( NNetModelWriterInterface & nmwi ) 
	{
		nmwi.GetUPShapes().DeselectAllShapes();
		m_selectedShapes.Apply2All([&](Shape &s){ s.Select(); });
	}

protected:
	ShapePtrList<Shape> m_selectedShapes;
	bool                m_bInitialized { false };
};

