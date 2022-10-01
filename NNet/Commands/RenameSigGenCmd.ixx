// RenameSigGenCmd.ixx
//
// Commands

module;

#include <string>
#include <memory>

export module RenameSigGenCmd;

import NNetCommand;
import NNetModel;

using std::wstring;
using std::make_unique;

export class RenameSigGenCmd : public NNetCommand
{
public:
	RenameSigGenCmd(SigGenId const id, wstring const& name)
		: m_pSigGen(m_pNMWI->GetSigGen(id)),
		m_wstrNameOld(m_pSigGen->GetName()),
		m_wstrNameNew(name)
	{}

	void Do() final
	{
		m_pSigGen->SetName(m_wstrNameNew);
	}

	void Undo() final
	{
		m_pSigGen->SetName(m_wstrNameOld);
	}

private:
	SignalGenerator* m_pSigGen;
	wstring           m_wstrNameOld;
	wstring           m_wstrNameNew;
};