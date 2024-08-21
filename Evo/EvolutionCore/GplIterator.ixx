// GplIterator.ixx
//
// EvolutionCore

module;

import Debug;

export module GplIterator;

import GridPoint;
import GridModel;

export class GplIterator
{
public:
    explicit GplIterator(GridModel const & grid) 
    :   m_grid(grid),
        m_gpCurrent(GP_NULL)
    { };

    GridPoint Begin()
    {
        m_gpCurrent = m_grid.GetOldestGp(); 
        return m_gpCurrent;
    }

    GridPoint GotoNext()
    {
        Assert(IsInGrid(m_gpCurrent));
        m_gpCurrent = m_grid.GetJuniorGp(m_gpCurrent);
        return m_gpCurrent;
    } 
    
    void MoveTo(GridPoint const gpNewPos) 
    { 
        m_gpCurrent = gpNewPos; 
    };

    GridPoint GetCurrent() const 
    { 
        return m_gpCurrent; 
    };

    bool IsAtEnd() const 
    { 
        return m_gpCurrent.IsNull(); 
    };
    
    bool IsNotAtEnd() const 
    {
        return m_gpCurrent.IsNotNull(); 
    };

private:
    GplIterator(GplIterator const &); // Disable copy constructor

//lint -e1725         // reference member
    GridModel const & m_grid;
//lint +e1725
    GridPoint    m_gpCurrent;
};
