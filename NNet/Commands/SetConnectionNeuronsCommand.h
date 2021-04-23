// SetConnectionNeuronsCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "MicroMeterPointVector.h"
#include "NNetModelWriterInterface.h"
#include "ShapePtrList.h"
#include "Command.h"
#include "ConnectionNeuron.h"

class SetConnectionNeuronsCommand : public Command
{
public:
	SetConnectionNeuronsCommand
	(
		MicroMeterPointVector const & umPntVector,
		unique_ptr<ShapeIdList>   upShapeIds
	)
	  : m_umPntVector(umPntVector),
		m_upShapeIds(move(upShapeIds))
	{}

	virtual void Do( NNetModelWriterInterface & nmwi ) 
	{ 
		nmwi.SetConnNeurons(m_umPntVector, *m_upShapeIds.get());
	}

private:
	MicroMeterPointVector   m_umPntVector;
	unique_ptr<ShapeIdList> m_upShapeIds;
};
