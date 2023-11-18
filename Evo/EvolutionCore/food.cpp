// Food.cpp
//
// EvolutionCore

#include <cassert>
#include <limits.h>

import Config;
import GridRect;
import GridNeighbor;
import Grid_model;
import GridField;

void GridModel::FoodGrowth()
{
	Apply2Grid
	(
    	[&](GridPoint const gp)
		{
            GridField & rGF = getGridField(gp);

            int const iLimit = rGF.GetFertility().GetValue() + rGF.GetFertilizer().GetValue();
            int const iDelta = iLimit - rGF.GetFoodStock().GetValue(); 

			if (iDelta != 0)
			{
				int iGrowth = (iDelta * m_enFoodGrowthRate.GetValue()) / 100;   // negative growth is possible
				if (iGrowth == 0)                                             // if foodstock is greater than maximum
					iGrowth = (iDelta > 0) ? 1 : -1;                            // caused by editor
				rGF.IncFoodStock(ENERGY_UNITS{ Cast2Short(iGrowth) });
			}

            rGF.ReduceFertilizer();
		}
	);
}
