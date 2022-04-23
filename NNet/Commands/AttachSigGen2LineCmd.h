// AttachSigGen2LineCmd.h
//
// Commands

#pragma once

#include "NobId.h"
#include "InputNeuron.h"
#include "SignalGenerator.h"
#include "NNetCommand.h"

class AttachSigGen2LineCmd : public NNetCommand
{
public:
	AttachSigGen2LineCmd(NobId const nobId)
	  : m_inputNeuron(*m_pNMWI->GetNobPtr<InputNeuron *>(nobId)),
		m_pSigGenOld(&m_inputNeuron.GetSigGen()),
		m_pSigGenNew(m_pNMWI->GetSigGenActive())
	{}

	void Do() final 
	{
		m_inputNeuron.SetSigGen(m_pSigGenNew);
	}

	void Undo() final 
	{
		m_inputNeuron.SetSigGen(m_pSigGenOld);
	}

private:
	InputNeuron     & m_inputNeuron;
	SignalGenerator * m_pSigGenOld;
	SignalGenerator * m_pSigGenNew;
};