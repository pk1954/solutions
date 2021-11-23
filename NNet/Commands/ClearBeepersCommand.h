// ClearBeepersCommand.h
//
// Commands

#pragma once

#include "NNetModelWriterInterface.h"
#include "NobId.h"
#include "NNetCommand.h"
#include "BaseKnot.h"

class ClearBeepersCommand : public NNetCommand
{
public:

	void clearTriggerSound(Neuron * const pNeuron) const
	{
		static const SoundDescr noSound { false, 0_Hertz, 0_MilliSecs };
		pNeuron->SetTriggerSound(noSound);
	}

	void clearAll(NNetModelWriterInterface & nmwi) const
	{
		nmwi.GetUPNobs().Apply2All<Neuron>([&](Neuron & n) { clearTriggerSound(& n); });
	}

	void clearAllSelected(NNetModelWriterInterface & nmwi) const
	{
		nmwi.GetUPNobs().Apply2AllSelected<Neuron>([&](Neuron & n) { clearTriggerSound(& n); });
	}

	void Do() final 
	{ 
		if (! m_bInitialized)
		{
			m_pNMWI->GetUPNobs().Apply2All<Neuron>
			(
				[&](Neuron & neuron) 
				{ 
					if (neuron.HasTriggerSound())
						m_beepers.push_back(Beeper{ & neuron, neuron.GetTriggerSound() });
				} 
			);
			m_bInitialized = true;
		}
		if (m_pNMWI->GetUPNobs().AnyNobsSelected())
			clearAllSelected(*m_pNMWI);
		else
			clearAll(*m_pNMWI);
	}

	virtual void Undo() 
	{ 
		clearAll(*m_pNMWI);
		for (Beeper const & beeper : m_beepers)
			beeper.m_pNeuron->SetTriggerSound(beeper.m_sound);
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
