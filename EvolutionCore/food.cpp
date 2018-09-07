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
    int iMaxFoodStock = 0;
    for ( unsigned int uiIndex = 0; uiIndex < list.GetLength(); ++ uiIndex )
    {
        int const iFoodstock = GetFoodStock( list.GetElement( uiIndex ) );
        assert( iFoodstock >= 0 );
        if ( iFoodstock > iMaxFoodStock )
            iMaxFoodStock = iFoodstock;
    }

    for ( unsigned int uiIndex = 0; uiIndex < list.GetLength(); ++ uiIndex )
    {
        if ( GetFoodStock( list.GetElement( uiIndex ) ) != iMaxFoodStock )
            list.RemoveFromList( uiIndex );
    }
}

void Grid::FoodGrowth( )
{
	int m_iRate = m_iFoodGrowthRate;

	m_lFoodGrowth = 0;

	Apply2Grid
	( 
    	[&](GridPoint const & gp)
		{
            GridField & rGF = getGridField( gp );

            int const iLimit = rGF.GetFertility() + rGF.GetFertilizer();
            int const iDelta = iLimit - rGF.GetFoodStock(); 

			if ( iDelta != 0 )
			{
				int iGrowth = (m_iRate * iDelta) / 100;   // negative growth is possible
				if ( iGrowth == 0 )                       // if foodstock is greater than maximum
					iGrowth = (iDelta > 0) ? 1 : -1;      // caused by editor
				m_lFoodGrowth += iGrowth;
				rGF.IncFoodStock( CastToShort(iGrowth) );
			}

            rGF.ReduceFertilizer( );
		}
	);
}
