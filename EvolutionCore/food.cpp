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

void ASSERT_PRODUCT( int const iA, int const iB )     
{                                       
    if ( iB > 0 )                      
        assert( iA < INT_MAX / iB ); 
}

void Grid::FoodGrowth( )
{
	int m_iRate = m_iFoodGrowthRate;

	m_uiFoodGrowth = 0;

	Apply2GridLambda
	( 
    	[&](GridPoint const & gp, short const s)
		{
            GridField & rGF  = getGridField( gp );
            int const   iMax = rGF.GetFertility() + rGF.GetFertilizer();
            if ( iMax > 0 )
            {
                int       iFood  = rGF.GetFoodStock();               
                int const iDelta = (iMax > iFood) ? (iMax - iFood) : 0; 

                #ifndef NDEBUG                      
                    ASSERT_PRODUCT( iFood, m_iRate );
                    ASSERT_PRODUCT( iFood * m_iRate, iDelta );
                    ASSERT_PRODUCT( iMax, 100 );
                #endif

                int iGrowth = (iFood * m_iRate * iDelta) / (iMax * 100) + 1;
                assert( iGrowth >= 0 );
                if ( iFood + iGrowth > iMax )
                    iGrowth = iMax - iFood;
                iFood += iGrowth;
                m_uiFoodGrowth += iGrowth;
                assert( iFood <= SHRT_MAX );
                assert( iFood >= SHRT_MIN );
                rGF.SetFoodStock( static_cast<short>(iFood) );
                rGF.ReduceFertilizer( );
			}
		},
		0
	);
}
