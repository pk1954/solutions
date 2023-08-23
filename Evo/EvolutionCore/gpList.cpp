// gpList.cpp : 
//

#pragma once


#include "dump.h"
#include "gridPoint.h"
#include "gridField.h"
#include "grid_model.h"

void GridPointList::CheckGridPointList( Grid const & grid ) const
{
//    DUMP::Dump( L" CheckGridPointList start" );

    if ( ListIsEmpty( ) )
    {
        assert( m_iCount == 0 );
        assert( GetYoungestGp( ).IsNull( ) );
//        DUMP::Dump( L" CheckGridPointList empty" );
        return;
    }

    GridPoint gpRun  = GetOldestGp( ); 
    int       iCount = 0;

    for (;;)
    {
        assert( grid.IsAlive( gpRun ) );
        ++ iCount;
//       DUMP::Dump( grid, gpRun );
//        DUMP::Flush( );
        if ( IsYoungest( gpRun ) )
            break;
        else
        {
            GridPoint gpJunior = grid.GetJuniorGp( gpRun );
            assert( IsInGrid( gpJunior ) );
            if ( grid.GetSeniorGp( gpJunior ) != gpRun )
            {
                DUMP::Dump( L" CheckGridPointList detected inconsistency" );
                assert( false );
            }
            gpRun = gpJunior;
        }
    } 
   
    assert( iCount == m_iCount );

//    DUMP::Dump( L" CheckGridPointList end" );
//	DUMP::DumpNL();
}

// Add: New element is added to start of list

void GridPointList::AddGridPointToList( Grid & grid, GridField & gf )
{
//  CHECK_GRIDPOINT_LIST( grid );
    
    GridPoint gp = gf.GetGridPoint( );

    gf.SetSeniorGp( m_gpYoungest );
    gf.SetJuniorGp( GP_NULL );

    if ( ListIsEmpty( ) )
        SetOldest( gp );
    else
        grid.SetJuniorGp( m_gpYoungest, gp );

    SetYoungest( gp );

    ++m_iCount;

//  CHECK_GRIDPOINT_LIST( grid );
}

void GridPointList::ReplaceGridPointInList( Grid & grid, GridField & gf, GridField & gfNew )
{
    GridPoint gpJunior = gf.GetJuniorGp( );
    GridPoint gpSenior = gf.GetSeniorGp( );
    GridPoint gp       = gf   .GetGridPoint( );
    GridPoint gpNew    = gfNew.GetGridPoint( );

    gfNew.SetJuniorGp( gpJunior );
    gfNew.SetSeniorGp( gpSenior );

    if ( IsYoungest( gp ) )
        SetYoungest( gpNew );
    else
        grid.SetSeniorGp( gpJunior, gpNew  );
              
    if ( IsOldest( gp ) )
        SetOldest( gpNew );
    else
        grid.SetJuniorGp( gpSenior, gpNew  );
              
    gf.CutConnections( );
//	CHECK_GRIDPOINT_LIST( grid );
}

// Delete: Element is deleted from list

void GridPointList::DeleteGridPointFromList( Grid & grid, GridField & gf )
{
//	CHECK_GRIDPOINT_LIST( grid );
	GridPoint gpJunior = gf.GetJuniorGp( );
    GridPoint gpSenior = gf.GetSeniorGp( );
    GridPoint gp       = gf.GetGridPoint( );
   
    if ( IsYoungest( gp ) ) // last one in list
        SetYoungest( gpSenior );
    else
        grid.SetSeniorGp( gpJunior, gpSenior );

    if ( IsOldest( gp ) ) // first one in list
        SetOldest( gpJunior );
    else
        grid.SetJuniorGp( gpSenior, gpJunior );

    gf.CutConnections( );

    --m_iCount;
//	CHECK_GRIDPOINT_LIST( grid );
}
