// SetTriggerSoundCommand.h
//
// Commands

#pragma once

#include "NNetModel.h"
#include "SoundInterface.h"
#include "ShapeId.h"
#include "Command.h"
#include "Neuron.h"

class SetTriggerSoundCommand : public Command
{
public:
	SetTriggerSoundCommand
	( 
		NNetModelWriterInterface * const   pModel, 
		ShapeId                    const   id, 
		SoundDescr                 const & sound
	)
	  : m_pNeuron( pModel->GetShapePtr<Neuron *>( id ) ),
		m_sound( sound )
	{ }

	virtual void Do( NNetModelWriterInterface * const pModel ) 
	{ 
		m_sound = m_pNeuron->SetTriggerSound( m_sound );
		pModel->ClearModel( );
	}

private:
	Neuron * const m_pNeuron;
	SoundDescr     m_sound;
};
