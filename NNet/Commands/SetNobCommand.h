// SetNobCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModelWriterInterface.h"
#include "MicroMeterPosDir.h"
#include "IoConnector.h"
#include "Command.h"
#include "NobId.h"
#include "Nob.h"

class SetNobCommand : public Command
{
public:
	SetNobCommand
	(
		Nob                & nob, 
		MicroMeterPosDir const posDir
	)
      : m_nob(nob),
		m_posDir(posDir)
	{}

	virtual void Do(NNetModelWriterInterface & nmwi) 
	{ 
		m_nob.RotateNob(m_nob .GetPos(), m_posDir.GetDir());
		m_nob.MoveNob  (m_posDir.GetPos() - m_nob.GetPos());
	}

	virtual void Undo(NNetModelWriterInterface & nmwi) 
	{ 
		m_nob.RotateNob(m_nob.GetPos(), -m_posDir.GetDir());
		m_nob.MoveNob  (m_nob.GetPos() - m_posDir.GetPos());
	}

private:
	MicroMeterPosDir const m_posDir;
	Nob                & m_nob;
};
