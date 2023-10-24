// food.cpp :
//


#include <cassert>
#include <limits.h>
#include "gridNeighbor.h"
#include "grid_model.h"
#include "gridField.h"

import Config;
import GridRect;

void Grid::FoodGrowth()
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
				rGF.IncFoodStock(ENERGY_UNITS{ CastToShort(iGrowth) });
			}

            rGF.ReduceFertilizer();
		}
	);
}
