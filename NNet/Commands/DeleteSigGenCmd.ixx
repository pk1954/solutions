// DeleteSigGenCmd.ixx
//
// Commands

module;

#include <vector>
#include <iostream>

export module DeleteSigGenCmd;

import SetActiveSigGenCmd;
import NNetCommand;

using std::vector;

export class DeleteSigGenCmd : public NNetCommand
{
public:
	DeleteSigGenCmd()
	{
		m_pNMWI->Apply2All<InputLine>
		(
			[this](InputLine & n)
			{
				if (n.GetSigGen() == m_pSigGenActive)
					m_affectedInputLines.push_back(&n);
			}
		);
	}

	void Do() final 
	{
		for (auto i : m_affectedInputLines)  // set all affected input lines to Standard
			i->SetSigGen(StdSigGen::Get());
		m_upSigGen = move(m_pNMWI->RemoveSigGen(m_sigGenId)); // sets active sigGen to standard
	}

	void Undo() final 
	{
		m_pNMWI->InsertSigGen(move(m_upSigGen), m_sigGenId);
		for (auto i : m_affectedInputLines)             // reset input lines
			i->SetSigGen(m_pSigGenActive);
		m_pNMWI->SetSigGenActive(m_sigGenId);
	}

	static void Register()
	{
		SymbolTable::ScrDefConst(NAME, &m_wrapper);
	}

	static void Push()
	{
		if (IsTraceOn())
			TraceStream() << NAME << endl;
		m_pStack->PushCommand(make_unique<DeleteSigGenCmd>());
	}

private:

	inline static const wstring NAME { L"DeleteSigGen" };

	inline static struct Wrapper : public ScriptFunctor
	{
		void operator() (Script& script) const final
		{
			DeleteSigGenCmd::Push();
		}
	} m_wrapper;

	SignalGenerator   * m_pSigGenActive { m_pNMWI->GetSigGenSelected() };
	SigGenId            m_sigGenId      { m_pNMWI->GetSigGenIdSelected() };
	UPSigGen            m_upSigGen;
	vector<InputLine *> m_affectedInputLines;
};