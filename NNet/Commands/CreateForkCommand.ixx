// CreateForkCommand.ixx
//
// Commands

module;

#include <memory>

export module CreateForkCommand;

import Types;
import NNetModel;
import AddPipe2PipeCommand;

using std::make_unique;

export class CreateForkCommand : public AddPipe2PipeCommand
{
public:
	CreateForkCommand   // case 7
	(
		NobId         const  idPipe,
		MicroMeterPnt const& pos
	)
		: AddPipe2PipeCommand(idPipe)
	{
		m_upBaseKnotInsert = make_unique<Fork>(pos);
		m_upExtPoint       = make_unique<OutputLine>(pos + m_pNMWI->OrthoVector(m_idPipe));
		m_upPipeOrtho      = make_unique<Pipe>(m_upBaseKnotInsert.get(), m_upExtPoint.get());
		m_upExtPoint      ->AddIncoming(*m_upPipeOrtho.get());
		m_upBaseKnotInsert->AddOutgoing(*m_upPipeOrtho.get());

		Initialize();
	}

	~CreateForkCommand() final = default;
};
