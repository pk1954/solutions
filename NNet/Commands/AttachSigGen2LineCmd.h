// AttachSigGen2LineCmd.h
//
// Commands

#pragma once

#include "NobId.h"
#include "InputLine.h"
#include "SignalGenerator.h"
#include "NNetCommand.h"

class AttachSigGen2LineCmd : public NNetCommand
{
public:
	AttachSigGen2LineCmd(NobId const nobId)
	  : m_inputLine(*m_pNMWI->GetNobPtr<InputLine *>(nobId)),
		m_pSigGenOld(m_inputLine.GetSigGen()),
		m_pSigGenNew(m_pNMWI->GetSigGenSelected())
	{}

	void Do() final 
	{
		m_inputLine.SetSigGen(m_pSigGenNew);
	}

	void Undo() final 
	{
		m_inputLine.SetSigGen(m_pSigGenOld);
	}

private:
	InputLine       & m_inputLine;
	SignalGenerator * m_pSigGenOld;
	SignalGenerator * m_pSigGenNew;
};