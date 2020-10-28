// MarkSelectionCommand.h
//
// Commands

#pragma once

#include <vector>
#include "BoolOp.h"
#include "NNetModelWriterInterface.h"
#include "Shape.h"
#include "Command.h"
#include "BaseKnot.h"

using std::vector;

class MarkSelectionCommand : public Command
{
public:
	MarkSelectionCommand( NNetModelWriterInterface & nmwi, tBoolOp const op )
	  : m_op( op )
	{
		nmwi.Apply2AllSelected<Shape>( [&](Shape & s) { m_selectedShapeIds.push_back( s.GetId() ); } ); 
	}

	virtual void Do( NNetModelWriterInterface & nmwi ) 
	{ 
		nmwi.Apply2AllSelected<Shape>( [&](Shape & s) { s.Mark( m_op ); } );
	}

	virtual void Undo( NNetModelWriterInterface & nmwi ) 
	{ 
		nmwi.Apply2All<Shape>( [&](Shape & s) { s.Mark( tBoolOp::opFalse ); } );
		for ( auto & idShape : m_selectedShapeIds ) { nmwi.MarkShape( idShape, tBoolOp::opTrue ); };
	}

private:
	vector<ShapeId> m_selectedShapeIds;
	tBoolOp const   m_op;
};

