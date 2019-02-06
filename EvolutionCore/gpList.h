// gpList.h : 
//

#pragma once

#include "gridPoint.h"
#include "gridField.h"

#ifdef _DEBUG
#define CHECK_GRIDPOINT_LIST( grid ) CheckGridPointList( grid )
#else 
#define CHECK_GRIDPOINT_LIST( grid )
#endif

class Grid;

//lint -sem(GridPointList::ResetGpList,initializer)

class GridPointList
{
public:
    GridPointList( ) 
        : m_gpOldest( GridPoint::NULL_VAL() ),
          m_gpYoungest( GridPoint::NULL_VAL() ),
          m_iCount( 0 )
    { };

    int GetSize( ) const { return m_iCount; }; 

    GridPoint const GetOldestGp  ( ) const { return m_gpOldest;   };
    GridPoint const GetYoungestGp( ) const { return m_gpYoungest; };

    bool ListIsEmpty( ) const { return m_gpOldest.IsNull( ); }

    void SetOldest  ( GridPoint const gp ) { m_gpOldest   = gp; }; 
    void SetYoungest( GridPoint const gp ) { m_gpYoungest = gp; }; 

    bool IsOldest  ( GridPoint const gp ) const { return m_gpOldest   == gp; }
    bool IsYoungest( GridPoint const gp ) const { return m_gpYoungest == gp; }

    void ResetGpList( )
    {
        m_gpOldest  .Set2Null( );
        m_gpYoungest.Set2Null( );
        m_iCount = 0;
    };

    void AddGridPointToList     ( Grid &, GridField & );
    void ReplaceGridPointInList ( Grid &, GridField &, GridField & );
    void DeleteGridPointFromList( Grid &, GridField & );

    void CheckGridPointList( Grid const & ) const;

private:
    GridPoint m_gpOldest;
    GridPoint m_gpYoungest;
    int       m_iCount;  // number of elements in list
};
