// CreateSynapseCommand.ixx
//
// Commands

module;

#include <memory>

export module CreateSynapseCommand;

import Types;
import NNetModel;
import NNetCommand;

using std::unique_ptr;
using std::make_unique;

export class CreateSynapseCommand : public NNetCommand
{
public:
	CreateSynapseCommand   // case 8
	(
		NobId         const  idPipe,
		MicroMeterPnt const& pos
	)
      : m_pPipe(m_pNMWI->GetNobPtr<Pipe*>(idPipe))
	{
		OutputLine outputLine(pos);
		m_upInputLine = make_unique<InputLine>(pos - m_pNMWI->OrthoVector(idPipe));
		m_upPipeOrtho = make_unique<Pipe>(m_upInputLine.get(), &outputLine);
		m_upSynapse   = make_unique<Synapse>(m_pPipe, m_upPipeOrtho.get());
		m_upSynapse  ->SetMainPipe(m_pPipe);
		m_upInputLine->SetPipe(m_upPipeOrtho.get());
		m_upPipeOrtho->SetEndPnt(m_upSynapse.get());
	}

	~CreateSynapseCommand() = default;

	void Do() final
	{
        m_pPipe->AddSynapse(m_upSynapse.get());
		m_pNMWI->Push2Model(move(m_upSynapse));
		m_pNMWI->Push2Model(move(m_upPipeOrtho));
		m_pNMWI->Push2Model(move(m_upInputLine));
	}

	void Undo() final
	{
		m_upInputLine = m_pNMWI->PopFromModel<InputLine>();
		m_upPipeOrtho = m_pNMWI->PopFromModel<Pipe>();
		m_upSynapse   = m_pNMWI->PopFromModel<Synapse>();
		m_pPipe->RemoveSynapse(m_upSynapse.get());
	}

private:

	Pipe * const          m_pPipe;
	unique_ptr<Pipe>      m_upPipe;
	unique_ptr<InputLine> m_upInputLine;
	unique_ptr<Pipe>      m_upPipeOrtho;
	unique_ptr<Synapse>   m_upSynapse;
};
