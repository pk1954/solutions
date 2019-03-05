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
    ENERGY_UNITS enMaxFoodStock = ENERGY_UNITS(0);
    for ( unsigned int uiIndex = 0; uiIndex < list.GetLength(); ++ uiIndex )
    {
        ENERGY_UNITS const enFoodstock = GetFoodStock( list.GetElement( uiIndex ) );
        assert(enFoodstock >= ENERGY_UNITS(0) );
        if ( enFoodstock >enMaxFoodStock )
           enMaxFoodStock =enFoodstock;
    }

    for ( unsigned int uiIndex = 0; uiIndex < list.GetLength(); ++ uiIndex )
    {
        if ( GetFoodStock( list.GetElement( uiIndex ) ) != enMaxFoodStock )
            list.RemoveFromList( uiIndex );
    }
}

void Grid::FoodGrowth( )
{
	int m_iRate = m_iFoodGrowthRate;

	m_enFoodGrowth = ENERGY_UNITS(0);

	Apply2Grid
	( 
    	[&](GridPoint const gp)
		{
            GridField & rGF = getGridField( gp );

            ENERGY_UNITS const iLimit = rGF.GetFertility() + rGF.GetFertilizer();
            ENERGY_UNITS const iDelta = iLimit - rGF.GetFoodStock(); 

			if ( iDelta != ENERGY_UNITS(0) )
			{
				ENERGY_UNITS iGrowth = (iDelta * m_iRate) / 100;                                // negative growth is possible
				if ( iGrowth == ENERGY_UNITS(0) )                                               // if foodstock is greater than maximum
					iGrowth = (iDelta > ENERGY_UNITS(0)) ? ENERGY_UNITS(1) : -ENERGY_UNITS(1);  // caused by editor
				m_enFoodGrowth += iGrowth;
				rGF.IncFoodStock( iGrowth );
			}

            rGF.ReduceFertilizer( );
		}
	);
}
