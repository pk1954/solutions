// AttachSigGen2ConnCmd.h
//
// Commands

#pragma once

#include "NobId.h"
#include "InputLine.h"
#include "InputConnector.h"
#include "SignalGenerator.h"
#include "NNetCommand.h"

class AttachSigGen2ConnCmd : public NNetCommand
{
public:
	AttachSigGen2ConnCmd(NobId const nobId)
	  : m_inputConnector(*m_pNMWI->GetNobPtr<InputConnector *>(nobId)),
		m_pSigGenOld(&m_inputConnector.GetSigGen()),
		m_pSigGenNew(m_pNMWI->GetSigGenActive())
	{}

	void Do() final 
	{
		m_inputConnector.SetSigGen(m_pSigGenNew);
	}

	void Undo() final 
	{
		m_inputConnector.SetSigGen(m_pSigGenOld);
	}

private:
	InputConnector  & m_inputConnector;
	SignalGenerator * m_pSigGenOld;
	SignalGenerator * m_pSigGenNew;
};