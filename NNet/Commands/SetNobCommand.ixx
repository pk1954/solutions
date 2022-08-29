// SetNobCommand.ixx
//
// Commands

module;

#include "NNetModelWriterInterface.h"
#include "IoConnector.h"
#include "Nob.h"

export module SetNobCommand;

import MoreTypes;
import NobId;
import NNetCommand;
import MicroMeterPosDir;

export class SetNobCommand : public NNetCommand
{
public:
	SetNobCommand
	(
		Nob                  & nob, 
		MicroMeterPosDir const posDir
	)
	  : m_nob(nob),
		m_posDir(posDir)
	{}

	void Do() final 
	{ 
		m_nob.RotateNob(m_nob .GetPos(), m_posDir.GetDir());
		m_nob.MoveNob  (m_posDir.GetPos() - m_nob.GetPos());
	}

	void Undo() final 
	{ 
		m_nob.RotateNob(m_nob.GetPos(), -m_posDir.GetDir());
		m_nob.MoveNob  (m_nob.GetPos() - m_posDir.GetPos());
	}

private:
	MicroMeterPosDir const m_posDir;
	Nob                  & m_nob;
};
