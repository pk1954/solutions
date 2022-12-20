// MoveSelectionCommand.ixx
//
// Commands

export module MoveSelectionCommand;

import Types;
import NNetCommand;
import NNetModel;

export class MoveSelectionCommand : public NNetCommand
{
public:
	explicit MoveSelectionCommand(MicroMeterPnt const & delta)
		: m_delta(delta)
	{ }

	void Do() final 
	{ 
		moveSelected(m_delta);
	}

	void Undo() final 
	{ 
		moveSelected(-m_delta);
	}

	void moveSelected(MicroMeterPnt const & delta)
	{
		m_pNMWI->GetUPNobs().Apply2AllSelected<PosNob>
		(
			[delta](PosNob& posNob)
			{
				if (!posNob.IsSynapse())         // Synapses are already implicitly moved 
					posNob.MoveNob(delta);		 // by m_pipeAdd
			}
		);
	}

	bool CombineCommands(Command const & src) final
	{ 
		MoveSelectionCommand const & srcCmd { static_cast<MoveSelectionCommand const &>(src) };
		m_delta += srcCmd.m_delta;
		return true; 
	};

private:
	MicroMeterPnt m_delta;
};
