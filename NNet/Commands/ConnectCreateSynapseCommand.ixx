// ConnectCreateSynapseCommand.ixx
//
// Commands

module;

#include <cassert>
#include <memory>

export module ConnectCreateSynapseCommand;

import Types;
import NNetCommand;
import NNetModel;

using std::unique_ptr;
using std::make_unique;

export class ConnectCreateSynapseCommand : public NNetCommand
{
public:
	ConnectCreateSynapseCommand   // case 2: OutputLine connects to Pipe
	(
		NobId const idOutputLine,
		NobId const idPipe
	)
      : m_idOutputLine(idOutputLine),
		m_pOutputLine (m_pNMWI->GetNobPtr<OutputLine*>(idOutputLine)),
		m_pPipe       (m_pNMWI->GetNobPtr<Pipe*>(idPipe))
	{
		assert(m_pOutputLine->IsOutputLine());
		m_upSynapse = make_unique<Synapse>(m_pPipe, &m_pOutputLine->GetPipe());
		m_upSynapse->Select(m_pOutputLine->IsSelected());
	}

	~ConnectCreateSynapseCommand() final = default;

	void Do() final
	{
		m_pNMWI->Push2Model(move(m_upSynapse));
		m_upOutputLine = m_pNMWI->RemoveFromModel<OutputLine>(m_idOutputLine);
	}

	void Undo() final
	{
		m_pNMWI->Restore2Model(move(m_upOutputLine));
		m_upSynapse = m_pNMWI->PopFromModel<Synapse>();
		m_pPipe->RemoveSynapse(m_upSynapse.get());
		m_pOutputLine->GetPipe().SetEndPnt(m_pOutputLine);
	}

private:
	NobId       const m_idOutputLine;
	OutputLine* const m_pOutputLine;
	Pipe*       const m_pPipe;

	unique_ptr<Synapse>    m_upSynapse;
	unique_ptr<OutputLine> m_upOutputLine;
};
