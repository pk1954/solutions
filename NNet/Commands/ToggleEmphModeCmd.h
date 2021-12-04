// ToggleEmphModeCmd.h
//
// Commands

#pragma once

#include "NNetModelWriterInterface.h"
#include "NobId.h"
#include "Pipe.h"
#include "NNetCommand.h"

class ToggleEmphModeCmd : public NNetCommand
{
public:
	ToggleEmphModeCmd(NobId const id)
	  :	m_idNob(id)
	{
		assert(IsDefined(m_idNob));
		assert(m_pNMWI->IsPipe(m_idNob));
	}

	void Do() final
	{ 
		Pipe & pipe { * m_pNMWI->GetNobPtr<Pipe *>(m_idNob) };
		pipe.Emphasize(!pipe.IsEmphasized());
	}

private:
	NobId const m_idNob;
};
