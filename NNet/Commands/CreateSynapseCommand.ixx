// CreateSynapseCommand.ixx
//
// Commands

module;

#include <memory>

export module CreateSynapseCommand;

import Types;
import NNetModel;
import AddPipe2PipeCommand;

using std::make_unique;

export class CreateSynapseCommand : public AddPipe2PipeCommand
{
public:
	CreateSynapseCommand   // case 8
	(
		NobId         const  idPipe,
		MicroMeterPnt const& pos
	)
		: AddPipe2PipeCommand(idPipe)
	{
		m_upBaseKnotInsert = make_unique<Synapse>(pos);
		m_upExtPoint       = make_unique<InputLine>(pos - m_pNMWI->OrthoVector(m_idPipe));
		m_upPipeOrtho      = make_unique<Pipe>(m_upExtPoint.get(), m_upBaseKnotInsert.get());
		m_upExtPoint      ->AddOutgoing(*m_upPipeOrtho.get());
		m_upBaseKnotInsert->AddIncoming(*m_upPipeOrtho.get());

		Initialize();
	}

	~CreateSynapseCommand() final = default;
};
