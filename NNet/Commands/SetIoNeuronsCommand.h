// SetIoNeuronsCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "MicroMeterPntVector.h"
#include "NNetModelWriterInterface.h"
#include "IoNeuronList.h"
#include "Command.h"
#include "IoNeuron.h"

class SetIoNeuronsCommand : public Command
{
public:
	SetIoNeuronsCommand
	(
		MicroMeterPntVector const & umPntVectorSrc,
		unique_ptr<NobIdList>       upNobIdsDst
	)
	  : m_umPntVectorSrc(umPntVectorSrc),
		m_upNobIdsDst(move(upNobIdsDst))
	{}

	virtual void Do(NNetModelWriterInterface & nmwi) 
	{ 
		nmwi.SetIoNeurons(m_umPntVectorSrc, *m_upNobIdsDst.get());
	}

private:
	MicroMeterPntVector   m_umPntVectorSrc;
	unique_ptr<NobIdList> m_upNobIdsDst;
};
