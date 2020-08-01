// ClearBeepersCommand.h
//
// Commands

#pragma once

#include "NNetModel.h"
#include "ShapeId.h"
#include "Command.h"
#include "BaseKnot.h"

class ClearBeepersCommand : public Command
{
public:
	ClearBeepersCommand( NNetModel * pModel )
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

	void clearAll( NNetModel * const pModel )
	{
		pModel->Apply2All<Neuron>( [&](Neuron & n) { clearTriggerSound( & n ); } );
	}

	void clearAllSelected( NNetModel * const pModel )
	{
		pModel->Apply2AllSelected<Neuron>( [&](Neuron & n) { clearTriggerSound( & n ); } );
	}

	virtual void Do( NNetModel * const pModel ) 
	{ 
		if ( pModel->AnyShapesSelected() )
			clearAllSelected( pModel );
		else
			clearAll( pModel );
	}

	virtual void Undo( NNetModel * const pModel ) 
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
