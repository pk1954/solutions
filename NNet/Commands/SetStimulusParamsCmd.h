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
		SignalGenerator & sigGen,
		fMicroSecs  const msec,
		fHertz      const freq
	)
	  : m_sigGen(sigGen),
		m_freqOld(sigGen.FreqMax()),
		m_freqNew(freq),
		m_msecOld(sigGen.TimeMax()),
		m_msecNew(msec)
	{ }

    void Do() final
	{ 
		m_sigGen.SetStimulusParams(m_msecNew, m_freqNew);
	}

	void Undo() final
	{ 
		m_sigGen.SetStimulusParams(m_msecOld, m_freqOld);
	}

	bool CombineCommands(Command const & src) final
	{ 
		SetStimulusParamsCmd const & srcCmd { static_cast<SetStimulusParamsCmd const &>(src) };
		if (&m_sigGen != &srcCmd.m_sigGen)
			return false;
		m_freqOld = srcCmd.m_freqOld;
		m_msecOld = srcCmd.m_msecOld;
		return true; 
	};

private:
	SignalGenerator & m_sigGen;
	fHertz            m_freqOld;
	fHertz      const m_freqNew;
	fMicroSecs        m_msecOld;
	fMicroSecs  const m_msecNew;
};
