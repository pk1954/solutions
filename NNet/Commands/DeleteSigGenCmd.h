// DeleteSigGenCmd.h
//
// Commands

#pragma once

#include "NNetModelWriterInterface.h"
#include "InputLine.h"
#include "SetActiveSigGenCmd.h"
#include "SignalGenerator.h"
#include "SigGenCommand.h"

using std::make_unique;

class DeleteSigGenCmd : public SigGenCommand
{
public:
	DeleteSigGenCmd()
	{
		m_sigGenId = m_pNMWI->GetSigGenIdActive();
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
		SignalGenerator * m_pSigGenStandard { m_pNMWI->GetSigGenStandard() };
		for (auto i : m_affectedInputLines)  // set all affected input lines to Standard
			i->SetSigGen(m_pSigGenStandard);
		m_upSigGen = move(m_pNMWI->RemoveSigGen(m_sigGenId));
		SetActiveSigGenId(STD_SIGGEN_ID);
	}

	void Undo() final 
	{
		m_pNMWI->InsertSigGen(move(m_upSigGen), m_sigGenId);
		for (auto i : m_affectedInputLines)             // reset input lines
			i->SetSigGen(m_pSigGenActive);
		SetActiveSigGenId(m_sigGenId);
	}

private:
	SignalGenerator     * m_pSigGenActive { m_pNMWI->GetSigGenActive() };
	SigGenId              m_sigGenId;
	UPSigGen              m_upSigGen;
	vector<InputLine *> m_affectedInputLines;
};