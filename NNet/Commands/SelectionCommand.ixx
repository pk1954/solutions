// SelectionCommand.ixx
//
// Commands

module;

#include <vector>
#include <memory>

export module NNetCommands:SelectionCommand;

export import :NNetCommand;

using std::unique_ptr;
using std::vector;

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
