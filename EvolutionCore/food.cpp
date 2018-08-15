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

            int iFood  = rGF.GetFoodStock();               
            int iLimit = rGF.GetFertility(); 
			iLimit += rGF.GetFertilizer();
			if ( iLimit > SHRT_MAX )
				iLimit = SHRT_MAX;

            int const iDelta = iLimit - iFood; 

			if ( iDelta != 0 )
			{
				int iGrowth = (m_iRate * iDelta) / 100;
				if ( iGrowth == 0 )
					iGrowth = (iDelta > 0) ? 1 : -1;
				iFood         += iGrowth;
				m_lFoodGrowth += iGrowth;
				assert( iFood <= SHRT_MAX );
				assert( iFood >= SHRT_MIN );
				rGF.SetFoodStock( static_cast<short>(iFood) );
			}

            rGF.ReduceFertilizer( );
		}
	);
}
