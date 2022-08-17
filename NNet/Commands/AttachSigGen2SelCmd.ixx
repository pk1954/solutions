// AttachSigGen2SelCmd.ixx
//
// Commands

module;

#include <vector>
#include "Nob.h"
#include "NobId.h"
#include "InputLine.h"
#include "InputConnector.h"
#include "SignalGenerator.h"
#include "NNetCommand.h"

export module AttachSigGen2SelCmd;

using std::vector;

export class AttachSigGen2SelCmd : public NNetCommand
{
public:
	AttachSigGen2SelCmd()
	{
		m_pNMWI->GetUPNobs().Apply2AllSelected<InputLine>
		(
			[this](Nob& nob) { push(nob); }
		);
	}

	void Do() final
	{
		m_pNMWI->GetUPNobs().Apply2AllSelected<InputLine>
			(
				[this](Nob& nob) { nob.SetSigGen(m_pSigGenNew); }
		);
	}

	void Undo() final
	{
		for (auto i : m_list)
		{
			m_pNMWI->GetNobPtr<InputLine*>(i.id)->SetSigGen(i.pSigGen);
		}
	}

private:
	struct SigGenAttachment
	{
		NobId             id;
		SignalGenerator* pSigGen;
	};
	vector<SigGenAttachment> m_list;
	SignalGenerator* m_pSigGenNew{ m_pNMWI->GetSigGenSelected() };

	void push(Nob& nob)
	{
		m_list.push_back(SigGenAttachment(nob.GetId(), nob.GetSigGen()));
	}
};