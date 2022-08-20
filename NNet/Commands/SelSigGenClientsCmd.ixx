// SelSigGenClientsCmd.ixx
//
// Commands

module;

#include "NNetModelWriterInterface.h"
#include "SignalGenerator.h"
#include "InputLine.h"

export module SelSigGenClientsCmd;

import SelectionCommand;

using std::make_unique;

export class SelSigGenClientsCmd : public SelectionCommand
{
public:
	SelSigGenClientsCmd()
	{
		m_pSigGen = m_pNMWI->GetSigGenSelected();
	}

	void Do() final
	{
		SelectionCommand::Do();
		m_pNMWI->DeselectAllNobs();
		m_pNMWI->GetUPNobs().Apply2All<InputLine>
			(
				[&](InputLine& n)
				{
					if (n.GetSigGen() == m_pSigGen)
					{
						n.Select(true);
						if (n.HasParentNob())
							n.GetParentNob()->Select(true);
					}
				}
		);
	}

private:
	SignalGenerator const* m_pSigGen;
};