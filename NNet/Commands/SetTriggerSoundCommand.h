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
		NNetModelWriterInterface & model, 
		ShapeId            const   id, 
		SoundDescr         const & sound
	)
	  : m_pNeuron( model.GetShapePtr<Neuron *>( id ) ),
		m_sound( sound )
	{ }

	virtual void Do( NNetModelWriterInterface & model )
	{ 
		m_sound = m_pNeuron->SetTriggerSound( m_sound );
		model.ClearModel( );
	}

private:
	Neuron * const m_pNeuron;
	SoundDescr     m_sound;
};
