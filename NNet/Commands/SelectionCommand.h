// SelectionCommand.h
//
// Commands

#pragma once

#include <vector>
#include "NNetModelWriterInterface.h"
#include "Shape.h"
#include "Command.h"

using std::vector;

class SelectionCommand : public Command
{
public:

	virtual void Do( NNetModelWriterInterface & nmwi ) 
	{
		if ( ! m_bInitialized )	
		{ 
			nmwi.GetUPShapes().Apply2AllSelected<Shape>
			( 
				[&](Shape &s) { m_selectedShapes.push_back(&s); } 
			);
			m_bInitialized = true;
		}
	}

	virtual void Undo( NNetModelWriterInterface & nmwi ) 
	{
		UPShapeList & shapeList { nmwi.GetUPShapes() };
		shapeList.DeselectAllShapes();
		for (auto pShape : m_selectedShapes)    
		{ 
			if (pShape)
				pShape->Select(); 
		}
	}

protected:
	vector<Shape *> m_selectedShapes;
	bool            m_bInitialized { false };
};

