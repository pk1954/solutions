// DeleteSigGenCmd.h
//
// Commands

#pragma once

#include "NNetModelWriterInterface.h"
#include "InputNeuron.h"
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
		m_pNMWI->Apply2All<InputNeuron>
		(
			[this](InputNeuron & n)
			{
				if (&n.GetSigGen() == m_pSigGenActive)
					m_affectedInputNeurons.push_back(&n);
			}
		);
	}

	void Do() final 
	{
		SignalGenerator * m_pSigGenStandard { m_pNMWI->GetSigGenStandard() };
		for (auto i : m_affectedInputNeurons)  // set all affected input lines to Standard
			i->SetSigGen(m_pSigGenStandard);
		m_upSigGen = move(m_pNMWI->RemoveSigGen(m_sigGenId));
		SetActiveSigGenId(STD_SIGGEN_ID);
	}

	void Undo() final 
	{
		m_pNMWI->InsertSigGen(move(m_upSigGen), m_sigGenId);
		for (auto i : m_affectedInputNeurons)             // reset input lines
			i->SetSigGen(m_pSigGenActive);
		SetActiveSigGenId(m_sigGenId);
	}

private:
	SignalGenerator     * m_pSigGenActive { m_pNMWI->GetSigGenActive() };
	SigGenId              m_sigGenId;
	UPSigGen              m_upSigGen;
	vector<InputNeuron *> m_affectedInputNeurons;
};