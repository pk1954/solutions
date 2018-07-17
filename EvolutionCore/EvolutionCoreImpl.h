#pragma once

#include <functional>
#include "EvolutionModelDataImpl.h"
#include "EvolutionCore.h"
#include "EditorState.h"

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

	virtual void DumpGridPointList( EvolutionModelData * const pModel ) const
	{
		dumpGridPointList( static_cast< EvolutionModelDataImpl * >( pModel )->m_grid );
	}

private:
    DisplayFunctor const * m_gridDisplayFunctor;    // GUI call back for display of current model 

    // private member functions

    EvolutionCoreImpl( EvolutionCoreImpl const & );               // noncopyable class 
    EvolutionCoreImpl & operator= ( EvolutionCoreImpl const & );  // noncopyable class 

	void dumpGridPointList( Grid const & ) const;
};
