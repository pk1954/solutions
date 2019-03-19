// EvolutionCoreImpl.h
//
// EvolutionCore

#pragma once

#include <functional>
#include "BoolOp.h"
#include "GridDimensions.h"
#include "gridPOI.h"
#include "gridPoint.h"
#include "gridRect.h"
#include "grid_model.h"
#include "gridBrush.h"
#include "EvolutionTypes.h"
#include "EvolutionCore.h"

class EvolutionCore;
class EventInterface;
class ObserverInterface;

class EvolutionCoreImpl : public EvolutionCore
{
public:
	static void EvolutionCoreImpl::InitClass
	( 
		ObserverInterface * const pObservers,
		EventInterface    * const pEvent
	)
	{
		m_pObservers = pObservers; 
		m_pEventPOI  = pEvent; 
	}

	EvolutionCoreImpl( );
    ~EvolutionCoreImpl( );

	// write access

	void EvolutionCoreImpl::CopyEvolutionCoreData( EvolutionCore const * const src )
	{
		* this = * static_cast<EvolutionCoreImpl const *>( src );
	}

    virtual void Compute( );
    
	virtual void ResetAll();

	virtual void SetBrushMode       ( tBrushMode   const mode  ) { m_brush.SetBrushMode  ( mode  );  }
	virtual void SetBrushManipulator( tManipulator const op    ) { m_brush.SetManipulator( op    );  }
	virtual void SetBrushShape      ( tShape       const shape ) { m_brush.SetShape      ( shape );  }
    virtual void SetBrushRadius     ( GRID_COORD   const rad   ) { m_brush.SetRadius     ( rad   );  }
    virtual void SetBrushIntensity  ( PERCENT      const perc  ) { m_brush.SetIntensity  ( perc  );  }
    virtual void SetSimulationMode  ( tBoolOp      const op    ) { ApplyOp( m_bSimulationMode, op ); }
    virtual void ModelDoEdit        ( GridPoint    const gp    ) { (m_brush)( gp ); }

	virtual PlannedActivity * GetPlan4Writing( ) { return & m_plan; };

	// read access

	virtual void DumpGridPointList( ) const;

	virtual bool GetSimulationMode( ) const { return m_bSimulationMode; }

    virtual EVO_GENERATION GetAge       ( GridPoint const gp ) const { return m_grid.GetAge( gp ); }

    virtual PERCENT        GetMutRate   ( GridPoint const gp ) const { return getGridField( gp ).GetMutRate( ); }
    virtual ENERGY_UNITS   GetFoodStock ( GridPoint const gp ) const { return getGridField( gp ).GetFoodStock( ); }
    virtual ENERGY_UNITS   GetFertility ( GridPoint const gp ) const { return getGridField( gp ).GetFertility( ); }
    virtual ENERGY_UNITS   GetFertilizer( GridPoint const gp ) const { return getGridField( gp ).GetFertilizer( ); }
    virtual EVO_GENERATION GetGenBirth  ( GridPoint const gp ) const { return getGridField( gp ).GetGenBirth( ); }
    virtual Strategy::Id   GetStrategyId( GridPoint const gp ) const { return getGridField( gp ).GetStrategyId( ); }
    virtual IND_ID         GetId        ( GridPoint const gp ) const { return getGridField( gp ).GetId( ); }
    virtual tOrigin        GetOrigin    ( GridPoint const gp ) const { return getGridField( gp ).GetOrigin( ); }
    virtual ENERGY_UNITS   GetEnergy    ( GridPoint const gp ) const { return getGridField( gp ).GetEnergy( ); }
    virtual MEM_INDEX      GetMemSize   ( GridPoint const gp ) const { return getGridField( gp ).GetMemSize( ); }
    virtual MEM_INDEX      GetMemUsed   ( GridPoint const gp ) const { return getGridField( gp ).GetMemUsed( ); }
    virtual bool           IsDead       ( GridPoint const gp ) const { return getGridField( gp ).IsDead( ); }
    virtual bool           IsAlive      ( GridPoint const gp ) const { return getGridField( gp ).IsAlive( ); }
    virtual bool           IsDefined    ( GridPoint const gp ) const { return getGridField( gp ).IsDefined( ); }

    virtual IND_ID         GetMemEntry  ( GridPoint const gp, MEM_INDEX    const index ) const { return getGridField( gp ).GetMemEntry( index ); }
    virtual short          GetAllele    ( GridPoint const gp, GeneType::Id const gene  ) const { return getGenome( gp ).GetAllele( gene ); }

	virtual EVO_GENERATION GetEvoGenerationNr ( ) const { return m_grid.GetEvoGenerationNr( ); }

	virtual tManipulator   GetBrushManipulator( ) const { return m_brush.GetManipulator(); }
    virtual PERCENT        GetBrushIntensity  ( ) const { return m_brush.GetIntensity(); }
    virtual tShape         GetBrushShape      ( ) const { return m_brush.GetShape(); }
    virtual GRID_COORD     GetBrushSize       ( ) const { return m_brush.GetRadius(); }
    virtual tBrushMode     GetBrushMode       ( ) const { return m_brush.GetBrushMode(); }

    virtual PlannedActivity const & GetPlan( ) const { return m_plan; };

	virtual int GetNrOfLivingIndividuals( ) const { return m_grid.GetNrOfLivingIndividuals( ); }

	virtual	ACTION_COUNT GetActionCounter
	(
		Strategy::Id const strategy,
		Action::Id   const action
	) const
	{
		return m_grid.GetActionCounter( strategy, action );
	}

	virtual GridPoint FindGridPoint( IND_ID const & ) const; 

	virtual GridPoint FindPOI( ) const; 

private:
    static ObserverInterface * m_pObservers;    // GUI call back for display of current model 
	static EventInterface    * m_pEventPOI;

	Grid            m_grid;	
    PlannedActivity m_plan;
	GridBrush 	    m_brush;
	bool	    	m_bSimulationMode;

	void stopOnPoi( GridPoint const, PlannedActivity & );
	bool IsPoi    ( GridPoint const gp ) { return  gp.IsNotNull( ) &&  GridPOI::IsPoi( GetId(gp) ); }

    GridField const & getGridField( GridPoint const gp ) const { return m_grid.GetGridField( gp ); }
    Genome    const & getGenome   ( GridPoint const gp ) const { return getGridField( gp ).GetGenome( ); }
};
