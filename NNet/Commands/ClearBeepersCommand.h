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
	ClearBeepersCommand( NNetModelWriterInterface * pModel )
	{
		pModel->Apply2All<Neuron>
			( 
				[&]( Neuron & neuron ) 
				{ 
					if ( neuron.HasTriggerSound() )
						m_beepers.push_back( Beeper{ & neuron, neuron.GetTriggerSound() } );
				} 
		);
	}

	void clearTriggerSound( Neuron * const pNeuron )
	{
		static const SoundDescr noSound { false, 0_Hertz, 0_MilliSecs };
		pNeuron->SetTriggerSound( noSound );
	}

	void clearAll( NNetModelWriterInterface * const pModel )
	{
		pModel->Apply2All<Neuron>( [&](Neuron & n) { clearTriggerSound( & n ); } );
	}

	void clearAllSelected( NNetModelWriterInterface * const pModel )
	{
		pModel->Apply2AllSelected<Neuron>( [&](Neuron & n) { clearTriggerSound( & n ); } );
	}

	virtual void Do( NNetModelWriterInterface * const pModel ) 
	{ 
		if ( pModel->GetModel().GetShapes().AnyShapesSelected() )
			clearAllSelected( pModel );
		else
			clearAll( pModel );
	}

	virtual void Undo( NNetModelWriterInterface * const pModel ) 
	{ 
		clearAll( pModel );
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
