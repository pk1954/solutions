// SetStimulusParamsCmd.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModelWriterInterface.h"
#include "NNetCommand.h"
#include "SignalGenerator.h"

class SetStimulusParamsCmd : public NNetCommand
{
public:
	SetStimulusParamsCmd
	(
		SignalGenerator       & sigGenDst,
		SignalGenerator const & sigGenSrc
	)
	  : m_sigGenDst(sigGenDst),
		m_sigGenOld(sigGenDst),
		m_sigGenSrc(sigGenSrc)
	{ }

    void Do() final
	{ 
		m_sigGenDst.SetParams(m_sigGenSrc);
	}

	void Undo() final
	{ 
		m_sigGenDst.SetParams(m_sigGenOld);
	}

	bool CombineCommands(Command const & src) final
	{ 
		SetStimulusParamsCmd const & srcCmd { static_cast<SetStimulusParamsCmd const &>(src) };
		if (&m_sigGenDst != &srcCmd.m_sigGenDst)
			return false;
		m_sigGenDst.SetParams(srcCmd.m_sigGenSrc); 
		return true; 
	};

private:
	SignalGenerator       & m_sigGenDst;
	SignalGenerator const   m_sigGenOld;
	SignalGenerator const & m_sigGenSrc;
};
