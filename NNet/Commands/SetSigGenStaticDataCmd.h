// SetSigGenStaticDataCmd.h
//
// Commands

#pragma once

#include "NNetModelWriterInterface.h"
#include "NNetCommand.h"
#include "SignalGenerator.h"

import MoreTypes;

class SetSigGenStaticDataCmd : public NNetCommand
{
public:
	SetSigGenStaticDataCmd
	(
		SignalGenerator  & sigGen,
		SigGenStaticData const & data
	)
	  : m_sigGen(sigGen),
		m_dataNew(data),
		m_dataOld(sigGen.GetStaticData())
	{ }

    void Do() final
	{ 
		m_sigGen.SetStaticData(m_dataNew);
	}

	void Undo() final
	{ 
		m_sigGen.SetStaticData(m_dataOld);
	}

	bool CombineCommands(Command const & src) final
	{ 
		SetSigGenStaticDataCmd const & srcCmd     { static_cast<SetSigGenStaticDataCmd const &>(src) };
		SignalGenerator        const * pSigGenSrc { &srcCmd.m_sigGen };
		if (&m_sigGen != pSigGenSrc)
			return false;
		m_dataNew = pSigGenSrc->GetStaticData();
		m_sigGen.SetStaticData(m_dataNew); 
		return true; 
	};

private:
	SignalGenerator       & m_sigGen;
	SigGenStaticData        m_dataNew;
	SigGenStaticData const  m_dataOld;
};
