#pragma once

#include <functional>
#include "BoolOp.h"
#include "gridPoint.h"
#include "gridRect.h"
#include "grid_model.h"
#include "gridBrush.h"
#include "EvolutionCore.h"

class EvolutionCore;
class EventInterface;
class ObserverInterface;

class EvolutionCoreImpl : public EvolutionCore
{
public:
	EvolutionCoreImpl( );
    ~EvolutionCoreImpl( );

	void EvolutionCoreImpl::CopyEvolutionCoreData( EvolutionCore const * const src )
	{
		* this = * static_cast<EvolutionCoreImpl const *>( src );
	}

    virtual void Compute( );
    
	virtual void SetObservers( ObserverInterface * const pObservers ) 
	{ 
		m_pObservers = pObservers; 
	}

	virtual void SetEvent( EventInterface * const pEvent ) 
	{ 
		m_pEventPOI = pEvent; 
	}

	virtual void DumpGridPointList( ) const;

	virtual void           ResetAll();

	virtual void           ResetSelection( )                       { m_gridRectSelection.Reset(); };
	virtual void           SetSelection  ( GridRect const & rect ) { m_gridRectSelection = rect; }

	virtual void           SetBrushMode       ( tBrushMode   const mode  ) { m_brush.SetBrushMode  ( mode  );  }
	virtual void           SetBrushManipulator( tManipulator const op    ) { m_brush.SetManipulator( op    );  }
	virtual void           SetBrushShape      ( tShape       const shape ) { m_brush.SetShape      ( shape );  }
    virtual void           SetBrushRadius     ( GRID_COORD   const rad   ) { m_brush.SetRadius     ( rad   );  }
    virtual void           SetBrushIntensity  ( short        const sInt  ) { m_brush.SetIntensity  ( sInt  );  }
    virtual void           SetSimulationMode  ( tBoolOp      const op    ) { ApplyOp( m_bSimulationMode, op ); }

    virtual void           ModelDoEdit        ( GridPoint  const gp )    { (m_brush)( gp ); }

	virtual bool           GetSimulationMode  ( ) const { return m_bSimulationMode; }

    virtual EVO_GENERATION GetAge       ( GridPoint const gp ) const { return m_grid.GetAge( gp ); }

    virtual int            GetFoodStock ( GridPoint const gp ) const { return getGridField( gp ).GetFoodStock( ); }
    virtual int            GetFertility ( GridPoint const gp ) const { return getGridField( gp ).GetFertility( ); }
    virtual int            GetMutRate   ( GridPoint const gp ) const { return getGridField( gp ).GetMutRate( ); }
    virtual int            GetFertilizer( GridPoint const gp ) const { return getGridField( gp ).GetFertilizer( ); }
    virtual EVO_GENERATION GetGenBirth  ( GridPoint const gp ) const { return getGridField( gp ).GetGenBirth( ); }
    virtual IndividualId   GetId        ( GridPoint const gp ) const { return getGridField( gp ).GetId( ); }
    virtual tOrigin        GetOrigin    ( GridPoint const gp ) const { return getGridField( gp ).GetOrigin( ); }
    virtual short          GetEnergy    ( GridPoint const gp ) const { return getGridField( gp ).GetEnergy( ); }
    virtual tStrategyId    GetStrategyId( GridPoint const gp ) const { return getGridField( gp ).GetStrategyId( ); }
    virtual MEM_INDEX      GetMemSize   ( GridPoint const gp ) const { return getGridField( gp ).GetMemSize( ); }
    virtual MEM_INDEX      GetMemUsed   ( GridPoint const gp ) const { return getGridField( gp ).GetMemUsed( ); }
    virtual bool           IsDead       ( GridPoint const gp ) const { return getGridField( gp ).IsDead( ); }
    virtual bool           IsAlive      ( GridPoint const gp ) const { return getGridField( gp ).IsAlive( ); }
    virtual bool           IsDefined    ( GridPoint const gp ) const { return getGridField( gp ).IsDefined( ); }

    virtual IndividualId   GetMemEntry  ( GridPoint const gp, MEM_INDEX const index ) const { return getGridField( gp ).GetMemEntry( index ); }
    virtual long           GetGenotype  ( GridPoint const gp, tGeneType const gene  ) const { return getGenome( gp ).GetAllele( gene ); }
    virtual short          GetDistr     ( GridPoint const gp, tAction   const at    ) const { return getGenome( gp ).GetDistr( at ); }

    virtual EVO_GENERATION GetEvoGenerationNr ( ) const { return m_grid.GetEvoGenerationNr( ); }

	virtual tManipulator   GetBrushManipulator( ) const { return m_brush.GetManipulator(); }
    virtual short          GetBrushIntensity  ( ) const { return m_brush.GetIntensity(); }
    virtual tShape         GetBrushShape      ( ) const { return m_brush.GetShape(); }
    virtual GRID_COORD     GetBrushSize       ( ) const { return m_brush.GetRadius(); }
    virtual tBrushMode     GetBrushMode       ( ) const { return m_brush.GetBrushMode(); }

	virtual GridRect       GetSelection       ( ) const { return m_gridRectSelection.IsEmpty( ) ? GridRect::GRID_RECT_FULL() : m_gridRectSelection; }
	virtual bool           SelectionIsEmpty   ( ) const { return m_gridRectSelection.IsEmpty(); }
	virtual bool           SelectionIsNotEmpty( ) const { return m_gridRectSelection.IsNotEmpty(); }

	virtual IndividualId   GetPoiId    ( )                         const { return m_idPOI; }
	virtual bool           IsPoiDefined( )                         const { return m_idPOI.IsDefined( ); }
	virtual bool           IsPoiId     ( IndividualId const & id ) const { return m_idPOI == id; }
	virtual bool           IsPoi       ( GridPoint    const   gp ) const { return ( gp.IsNotNull( ) && ( GetId( gp ) == m_idPOI ) ); }
	virtual void           ClearPoi    ( )                               { m_idPOI.ResetIndId( ); }
	 
    virtual PlannedActivity const & GetPlan( )         const { return   m_plan; };
    virtual PlannedActivity       * GetPlan4Writing( )       { return & m_plan; };

	virtual long GetGridArea( ) const { return GRID_AREA(); };

    virtual int GetAverageFoodGrowth( )     const { return m_grid.GetAverageFoodGrowth( ); }
    virtual int GetNrOfLivingIndividuals( ) const { return m_grid.GetNrOfLivingIndividuals( ); }

	virtual	unsigned int GetActionCounter
	(
		unsigned int const uiStrategy,
		tAction      const action
	) const
	{
		return m_grid.GetActionCounter( uiStrategy, action );
	}

	virtual void SetPoi( GridPoint const );
    virtual GridPoint FindPOI( ) const;
	virtual GridPoint FindGridPoint( IndividualId const & id, GridRect const & rect = GridRect::GRID_RECT_FULL() ) const 
	{ 
		return ( id == IndividualId::NO_INDIVIDUAL )
			   ? GridPoint::NULL_VAL()
			   : m_grid.FindGridPoint( [&](GridPoint const gp) { return (GetId(gp) == id); }, rect );
	}

private:
    ObserverInterface * m_pObservers;    // GUI call back for display of current model 
	EventInterface    * m_pEventPOI;
	Grid                m_grid;	
    PlannedActivity     m_plan;
    IndividualId        m_idPOI;
	GridBrush 	        m_brush;
	bool	    	    m_bSimulationMode;
	GridRect            m_gridRectSelection;

	void stopOnPoi( GridPoint const, PlannedActivity & );

    GridField const & getGridField( GridPoint const gp ) const { return m_grid.GetGridField( gp ); }
    Genome    const & getGenome   ( GridPoint const gp ) const { return getGridField( gp ).GetGenome( ); }
};
