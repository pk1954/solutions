// food.cpp :
//

#include "stdafx.h"
#include <assert.h>
#include <limits.h>
#include "config.h"
#include "gridNeighbor.h"
#include "gridRect.h"
#include "grid_model.h"
#include "gridField.h"

// returns a list of GRIDPOINTS in pgpList and length of list as return value

void Grid::getBestNeighborSlots( Neighborhood & list )
{
    ENERGY_UNITS enMaxFoodStock = 0_ENERGY_UNITS;
    for ( unsigned int uiIndex = 0; uiIndex < list.GetLength(); ++ uiIndex )
    {
        ENERGY_UNITS const enFoodstock = GetFoodStock( list.GetElement( uiIndex ) );
        assert(enFoodstock >= 0_ENERGY_UNITS );
        if ( enFoodstock > enMaxFoodStock )
           enMaxFoodStock = enFoodstock;
    }

    for ( unsigned int uiIndex = 0; uiIndex < list.GetLength(); ++ uiIndex )
    {
        if ( GetFoodStock( list.GetElement( uiIndex ) ) != enMaxFoodStock )
            list.RemoveFromList( uiIndex );
    }
}

void Grid::FoodGrowth( )
{
	m_enFoodGrowth = 0_ENERGY_UNITS;

	Apply2Grid
	( 
    	[&](GridPoint const gp)
		{
            GridField & rGF = getGridField( gp );

            ENERGY_UNITS const iLimit = rGF.GetFertility() + rGF.GetFertilizer();
            ENERGY_UNITS const iDelta = iLimit - rGF.GetFoodStock(); 

			if ( iDelta != 0_ENERGY_UNITS )
			{
				ENERGY_UNITS enGrowth = (iDelta * m_enFoodGrowthRate.GetValue()) / 100;                                // negative growth is possible
				if ( enGrowth == 0_ENERGY_UNITS )                                               // if foodstock is greater than maximum
					enGrowth = (iDelta > 0_ENERGY_UNITS) ? 1_ENERGY_UNITS : -1_ENERGY_UNITS;  // caused by editor
				m_enFoodGrowth += enGrowth;
				rGF.IncFoodStock( enGrowth );
			}

            rGF.ReduceFertilizer( );
		}
	);
}
