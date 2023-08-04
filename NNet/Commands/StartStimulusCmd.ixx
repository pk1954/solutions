// StartStimulusCmd.ixx
//
// Commands

module;

#include <iostream>

export module NNetCommands:StartStimulusCmd;

import :NNetCommand;

export class StartStimulusCmd : public NNetCommand
{
public:

	StartStimulusCmd()
	{
	}

	void Do() final
	{
	};

	void Undo() final
	{
	};

	static void Register()
	{
		SymbolTable::ScrDefConst(NAME, &m_wrapper);
	}

	static void Push()
	{
		if (IsTraceOn())
			TraceStream() << NAME << endl;
		m_pNMWI->GetSigGenSelected()->StartStimulus();
	}

private:

	inline static const wstring NAME { L"StartStimulus" };

	inline static struct Wrapper : public WrapBase
	{
		using WrapBase::WrapBase;
		void operator() (Script& script) const final
		{
			StartStimulusCmd::Push();
		}
	} m_wrapper { NAME };

};
