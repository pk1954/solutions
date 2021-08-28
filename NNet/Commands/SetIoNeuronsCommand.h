// SetIoNeuronsCommand.h
//
// Commands

#pragma once

//#include "MoreTypes.h"
//#include "MicroMeterPntVector.h"
//#include "NNetModelWriterInterface.h"
//#include "Command.h"
//#include "IoNeuron.h"
//
//class SetIoNeuronsCommand : public Command
//{
//public:
//	SetIoNeuronsCommand
//	(
//		MicroMeterPntVector const & umPntVectorSrc,
//		unique_ptr<NobIdList>       upNobIdsDst
//	)
//	  : m_umPntVectorSrc(umPntVectorSrc),
//		m_upNobIdsDst(move(upNobIdsDst))
//	{}
//
//	virtual void Do() 
//	{ 
//		unsigned int ui = 0;
//		m_upNobIdsDst->Apply2All
//		(
//			[&](NobId const & id) { m_pNMWI->SetPosDir(id, m_umPntVectorSrc.GetPosDir(ui++)); }
//		);
//	}
//
//	virtual void Undo() 
//	{ 
//		Do();   //TODO: check
//	}
//
//private:
//	MicroMeterPntVector   m_umPntVectorSrc;
//	unique_ptr<NobIdList> m_upNobIdsDst;
//};
