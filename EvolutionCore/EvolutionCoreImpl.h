#pragma once

#include "EvolutionCore.h"

class EvolutionModelData;
class DisplayFunctor;

class EvolutionCoreImpl : public EvolutionCore
{
public:
    explicit EvolutionCoreImpl( );
    ~EvolutionCoreImpl( );

    virtual void Compute( EvolutionModelData * const );
    
	virtual void SetGridDisplayFunctor( DisplayFunctor const * const f ) 
	{ 
		m_gridDisplayFunctor = f; 
	}

	virtual void DumpGridPointList( EvolutionModelData * const ) const;

private:
    DisplayFunctor const * m_gridDisplayFunctor;    // GUI call back for display of current model 

    EvolutionCoreImpl( EvolutionCoreImpl const & );               // noncopyable class 
    EvolutionCoreImpl & operator= ( EvolutionCoreImpl const & );  // noncopyable class 
};
