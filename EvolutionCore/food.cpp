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

ENERGY_UNITS Grid::getBestNeighborSlots( Neighborhood & list )
{
    ENERGY_UNITS enMaxFoodStock = 0_ENERGY_UNITS;
    for ( unsigned int uiIndex = 0; uiIndex < list.GetLength(); ++ uiIndex )
    {
        ENERGY_UNITS const enFoodstock = GetFoodStock( list.GetElement( uiIndex ) );
        assert(enFoodstock >= 0_ENERGY_UNITS );
        if ( enFoodstock > enMaxFoodStock )
           enMaxFoodStock = enFoodstock;
    }

	list.RemoveFromList
	(
		[&](GridPoint const gp)
		{
			return ( GetFoodStock( gp ) != enMaxFoodStock );
		}
	);
	return enMaxFoodStock;
}

void Grid::FoodGrowth( )
{
	Apply2Grid
	( 
    	[&](GridPoint const gp)
		{
            GridField & rGF = getGridField( gp );

            int const iLimit = rGF.GetFertility().GetValue() + rGF.GetFertilizer().GetValue();
            int const iDelta = iLimit - rGF.GetFoodStock().GetValue(); 

			if ( iDelta != 0 )
			{
				int iGrowth = (iDelta * m_enFoodGrowthRate.GetValue()) / 100;   // negative growth is possible
				if ( iGrowth == 0 )                                             // if foodstock is greater than maximum
					iGrowth = (iDelta > 0) ? 1 : -1;                            // caused by editor
				rGF.IncFoodStock( ENERGY_UNITS{ CastToShort(iGrowth) } );
			}

            rGF.ReduceFertilizer( );
			return false;
		}
	);
}
