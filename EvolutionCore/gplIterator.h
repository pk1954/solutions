// gplIterator.h : 
//

#pragma once

#include "assert.h"
#include "gridPoint.h"

class Grid;

class GplIterator
{
public:
    explicit GplIterator( Grid const & grid ) 
    :   m_grid( grid ),
        m_gpCurrent( GridPoint::NULL_VAL() )
    { };

    GridPoint Begin( )
    {
        m_gpCurrent = m_grid.GetOldestGp( ); 
        return m_gpCurrent;
    }

    GridPoint GotoNext( )
    {
        assert( IsInGrid( m_gpCurrent ) );
        m_gpCurrent = m_grid.GetJuniorGp( m_gpCurrent );
        return m_gpCurrent;
    } 
    
    void MoveTo( GridPoint const gpNewPos ) 
    { 
        m_gpCurrent = gpNewPos; 
    };

    GridPoint GetCurrent( ) const 
    { 
        return m_gpCurrent; 
    };

    bool IsAtEnd( ) const 
    { 
        return m_gpCurrent.IsNull( ); 
    };
    
    bool IsNotAtEnd( ) const 
    {
        return m_gpCurrent.IsNotNull( ); 
    };

private:
    GplIterator( GplIterator const & ); // Disable copy constructor

//lint -e1725         // reference member
    Grid const & m_grid;
//lint +e1725
    GridPoint    m_gpCurrent;
};
