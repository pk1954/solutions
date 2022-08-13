// SelectionCommand.ixx
//
// Commands

module;

#include <vector>
#include "Nob.h"
#include "NNetModelWriterInterface.h"
#include "NNetCommand.h"

export module SelectionCommand;

using std::unique_ptr;

export class SelectionCommand : public NNetCommand
{
public:

	void Undo() override
	{
		m_pNMWI->DeselectAllNobs();
		for (auto it : *m_upSelectedNobs){ it->Select(true); };
	}

private:
	unique_ptr<vector<Nob *>> m_upSelectedNobs { m_pNMWI->GetSelection() };
};

