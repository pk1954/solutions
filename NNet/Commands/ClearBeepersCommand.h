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

	void clearTriggerSound( Neuron * const pNeuron ) const
	{
		static const SoundDescr noSound { false, 0_Hertz, 0_MilliSecs };
		pNeuron->SetTriggerSound( noSound );
	}

	void clearAll( NNetModelWriterInterface & nmwi ) const
	{
		nmwi.GetShapes().Apply2All<Neuron>( [&](Neuron & n) { clearTriggerSound( & n ); } );
	}

	void clearAllSelected( NNetModelWriterInterface & nmwi ) const
	{
		nmwi.GetShapes().Apply2AllSelected<Neuron>( [&](Neuron & n) { clearTriggerSound( & n ); } );
	}

	virtual void Do( NNetModelWriterInterface & nmwi ) 
	{ 
		if ( ! m_bInitialized )
		{
			nmwi.GetShapes().Apply2All<Neuron>
			( 
				[&]( Neuron & neuron ) 
				{ 
					if ( neuron.HasTriggerSound() )
						m_beepers.push_back( Beeper{ & neuron, neuron.GetTriggerSound() } );
				} 
			);
			m_bInitialized = true;
		}
		if ( nmwi.GetModel().GetShapes().AnyShapesSelected() )
			clearAllSelected( nmwi );
		else
			clearAll( nmwi );
	}

	virtual void Undo( NNetModelWriterInterface & nmwi ) 
	{ 
		clearAll( nmwi );
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
	bool           m_bInitialized { false };
};
