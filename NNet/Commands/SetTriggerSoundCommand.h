// SetTriggerSoundCommand.h
//
// Commands

#pragma once

#include "NNetModelWriterInterface.h"
#include "SoundInterface.h"
#include "NobId.h"
#include "Command.h"
#include "Neuron.h"

class SetTriggerSoundCommand : public Command
{
public:
	SetTriggerSoundCommand
	(
		NobId      const   id, 
		SoundDescr const & sound
	)
	  : m_idNeuron(id),
		m_sound(sound)
	{ }

	virtual void Do()
	{ 
		m_sound = m_pNMWI->GetNobPtr<Neuron *>(m_idNeuron)->SetTriggerSound(m_sound);
		m_pNMWI->ClearAllNobs();
	}

	virtual void Undo()
	{ 
		Do();
	}

private:
	NobId const m_idNeuron;
	SoundDescr  m_sound;
};
