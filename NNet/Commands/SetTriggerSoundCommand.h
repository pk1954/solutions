// SetTriggerSoundCommand.h
//
// Commands

#pragma once

#include "NNetModelWriterInterface.h"
#include "SoundInterface.h"
#include "ShapeId.h"
#include "Command.h"
#include "Neuron.h"

class SetTriggerSoundCommand : public Command
{
public:
	SetTriggerSoundCommand
	( 
		ShapeId    const   id, 
		SoundDescr const & sound
	)
	  : m_idNeuron( id ),
		m_sound( sound )
	{ }

	virtual void Do( NNetModelWriterInterface & nmwi )
	{ 
		m_sound = nmwi.GetShapePtr<Neuron *>( m_idNeuron )->SetTriggerSound( m_sound );
		nmwi.ClearModel( );
	}

private:
	ShapeId const m_idNeuron;
	SoundDescr    m_sound;
};
