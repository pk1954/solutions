// SetConnectionNeuronsCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "MicroMeterPointVector.h"
#include "NNetModelWriterInterface.h"
#include "NobPtrList.h"
#include "Command.h"
#include "ConnectionNeuron.h"

class SetConnectionNeuronsCommand : public Command
{
public:
	SetConnectionNeuronsCommand
	(
		MicroMeterPointVector const & umPntVector,
		unique_ptr<NobIdList>   upNobIds
	)
	  : m_umPntVector(umPntVector),
		m_upNobIds(move(upNobIds))
	{}

	virtual void Do( NNetModelWriterInterface & nmwi ) 
	{ 
		nmwi.SetConnNeurons(m_umPntVector, *m_upNobIds.get());
	}

private:
	MicroMeterPointVector   m_umPntVector;
	unique_ptr<NobIdList> m_upNobIds;
};
