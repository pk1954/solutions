// ToggleEmphModeCmd.ixx
//
// Commands

module;

#include "NNetModelWriterInterface.h"
#include "Pipe.h"
#include "NNetCommand.h"

export module ToggleEmphModeCmd;

import NobId;

export class ToggleEmphModeCmd : public NNetCommand
{
public:
	explicit ToggleEmphModeCmd(NobId const id)
		: m_pPipe(m_pNMWI->GetNobPtr<Pipe*>(id))
	{
		assert(m_pPipe);
	}

	void Do() final
	{ 
		m_pPipe->Emphasize(!m_pPipe->IsEmphasized());
	}

	void Undo() final
	{ 
		Do();
	}

private:
	Pipe * m_pPipe { nullptr };
};
