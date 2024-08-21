// Strategy.cpp : 
//
// EvolutionCore

module;

import Debug;

module Strategy;

import Random;
import StrategyData;
import Strategy;

bool Tit4Tat::InteractWith(StrategyData &data, IND_ID const idPartner) const
{
    bool const bNotorious = data.FindInListOfCulprits(idPartner).IsNotNull();
    return ! bNotorious;                                    // If he is not in the list of bad guys, be friendly
}

void Tit4Tat::Remember(StrategyData &data, IND_ID const idPartner, bool const bPartnerReaction) const
{
    MEM_INDEX const index = data.FindInListOfCulprits(idPartner);

    if (index.IsNotNull())               // He is on the list of bad guys
    {
        if (bPartnerReaction)            // He turned into a good guy. Let him off the hook
        {
            data.RemoveFromListOfCulprits(index);
        }
        else                               // He is persistently bad. Remains on the list.
        {
			data.KnownCulprit();
        }
    }
    else 	                               // We don't know him
    {
        if (bPartnerReaction)              // He seems to be a good guy
        {
        }
        else                               // He doesn't behave well. Put him on the list
        {
            data.AddToListOfCulprits(idPartner);
			data.UnknownCulprit();
        }
    }
};

