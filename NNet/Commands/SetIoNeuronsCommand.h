// SetIoNeuronsCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "MicroMeterPntVector.h"
#include "NNetModelWriterInterface.h"
#include "NobPtrList.h"
#include "Command.h"
#include "IoNeuron.h"

class SetIoNeuronsCommand : public Command
{
public:
	SetIoNeuronsCommand
	(
		MicroMeterPntVector const & umPntVector,
		unique_ptr<NobIdList>   upNobIds
	)
	  : m_umPntVector(umPntVector),
		m_upNobIds(move(upNobIds))
	{}

	virtual void Do( NNetModelWriterInterface & nmwi ) 
	{ 
		nmwi.SetIoNeurons(m_umPntVector, *m_upNobIds.get());
	}

private:
	MicroMeterPntVector   m_umPntVector;
	unique_ptr<NobIdList> m_upNobIds;
};
