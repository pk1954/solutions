// SetSigGenDataCmd.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModelWriterInterface.h"
#include "NNetCommand.h"
#include "SignalGenerator.h"

class SetSigGenDataCmd : public NNetCommand
{
public:
	SetSigGenDataCmd
	(
		SignalGenerator  & sigGen,
		SigGenData const & data
	)
	  : m_sigGen(sigGen),
		m_dataNew(data),
		m_dataOld(sigGen.GetData())
	{ }

    void Do() final
	{ 
		m_sigGen.SetData(m_dataNew);
	}

	void Undo() final
	{ 
		m_sigGen.SetData(m_dataOld);
	}

	bool CombineCommands(Command const & src) final
	{ 
		SetSigGenDataCmd const & srcCmd     { static_cast<SetSigGenDataCmd const &>(src) };
		SignalGenerator  const * pSigGenSrc { &srcCmd.m_sigGen };
		if (&m_sigGen != pSigGenSrc)
			return false;
		m_dataNew = pSigGenSrc->GetData();
		m_sigGen.SetData(m_dataNew); 
		return true; 
	};

private:
	SignalGenerator & m_sigGen;
	SigGenData        m_dataNew;
	SigGenData const  m_dataOld;
};
