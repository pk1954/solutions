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
	explicit AttachSigGen2ConnCmd(NobId const nobId)
	  : m_inputConnector(*m_pNMWI->GetNobPtr<InputConnector *>(nobId)),
		m_pSigGenOld(m_inputConnector.GetSigGen())
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
	SignalGenerator * m_pSigGenNew { m_pNMWI->GetSigGenSelected() };
	InputConnector  & m_inputConnector;
	SignalGenerator * m_pSigGenOld;
};