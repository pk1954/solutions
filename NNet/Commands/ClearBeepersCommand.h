// ClearBeepersCommand.h
//
// Commands

#pragma once

#include "NNetModelWriterInterface.h"
#include "ShapeId.h"
#include "Command.h"
#include "BaseKnot.h"

class ClearBeepersCommand : public Command
{
public:
	ClearBeepersCommand( NNetModelWriterInterface & model )
	{
		model.Apply2All<Neuron>
			( 
				[&]( Neuron & neuron ) 
				{ 
					if ( neuron.HasTriggerSound() )
						m_beepers.push_back( Beeper{ & neuron, neuron.GetTriggerSound() } );
				} 
		);
	}

	void clearTriggerSound( Neuron * const pNeuron ) const
	{
		static const SoundDescr noSound { false, 0_Hertz, 0_MilliSecs };
		pNeuron->SetTriggerSound( noSound );
	}

	void clearAll( NNetModelWriterInterface & model ) const
	{
		model.Apply2All<Neuron>( [&](Neuron & n) { clearTriggerSound( & n ); } );
	}

	void clearAllSelected( NNetModelWriterInterface & model ) const
	{
		model.Apply2AllSelected<Neuron>( [&](Neuron & n) { clearTriggerSound( & n ); } );
	}

	virtual void Do( NNetModelWriterInterface & model ) 
	{ 
		if ( model.GetModel().GetShapes().AnyShapesSelected() )
			clearAllSelected( model );
		else
			clearAll( model );
	}

	virtual void Undo( NNetModelWriterInterface & model ) 
	{ 
		clearAll( model );
		for ( Beeper const & beeper : m_beepers )
			beeper.m_pNeuron->SetTriggerSound( beeper.m_sound );
	}

private:
	struct Beeper
	{
		Neuron   * m_pNeuron;
		SoundDescr m_sound;
	};
	vector<Beeper> m_beepers;
};
