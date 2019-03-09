// EvolutionCore.h
//

#pragma once

#include "BoolOp.h"
#include "GridRect.h"
#include "EvolutionTypes.h"
#include "GridDimensions.h"

class ObserverInterface;
class EventInterface;
class EvolutionCore;
class PlannedActivity;

class EvolutionCore
{
public:

    virtual ~EvolutionCore( ) { };

	virtual void CopyEvolutionCoreData( EvolutionCore const * const ) = 0;

// debugging functions

    virtual void DumpGridPointList( ) const = 0;

// readOnly functions

    virtual int            GetNrOfLivingIndividuals ( ) const = 0;
    virtual ENERGY_UNITS   GetAverageFoodGrowth     ( ) const = 0;
    virtual EVO_GENERATION GetEvoGenerationNr       ( ) const = 0;
    virtual PERCENT        GetBrushIntensity        ( ) const = 0;
    virtual tShape         GetBrushShape            ( ) const = 0;
    virtual tManipulator   GetBrushManipulator      ( ) const = 0;
    virtual GRID_COORD     GetBrushSize             ( ) const = 0;
    virtual tBrushMode     GetBrushMode             ( ) const = 0;
	virtual GridRect       GetSelection             ( ) const = 0;
	virtual bool           GetSimulationMode        ( ) const = 0;
	virtual bool           SelectionIsEmpty         ( ) const = 0;
	virtual bool           SelectionIsNotEmpty      ( ) const = 0;

    virtual bool           IsDead       ( GridPoint const ) const = 0;
    virtual bool           IsAlive      ( GridPoint const ) const = 0;
    virtual bool           IsDefined    ( GridPoint const ) const = 0;
    virtual IND_ID         GetId        ( GridPoint const ) const = 0;
    virtual tOrigin        GetOrigin    ( GridPoint const ) const = 0;
    virtual ENERGY_UNITS   GetEnergy    ( GridPoint const ) const = 0;
    virtual ENERGY_UNITS   GetFoodStock ( GridPoint const ) const = 0;
    virtual ENERGY_UNITS   GetFertility ( GridPoint const ) const = 0;
    virtual PERCENT        GetMutRate   ( GridPoint const ) const = 0;
    virtual ENERGY_UNITS   GetFertilizer( GridPoint const ) const = 0;
    virtual EVO_GENERATION GetGenBirth  ( GridPoint const ) const = 0;
    virtual EVO_GENERATION GetAge       ( GridPoint const ) const = 0;
    virtual Strategy::Id   GetStrategyId( GridPoint const ) const = 0;
    virtual MEM_INDEX      GetMemSize   ( GridPoint const ) const = 0;
    virtual MEM_INDEX      GetMemUsed   ( GridPoint const ) const = 0;
    virtual IND_ID         GetMemEntry  ( GridPoint const, MEM_INDEX           const ) const = 0;
    virtual short          GetDistr     ( GridPoint const, GeneType::Id const ) const = 0;
    virtual long           GetGenotype  ( GridPoint const, GeneType::Id const ) const = 0;
	
	virtual	ACTION_COUNT   GetActionCounter( Strategy::Id const, Action::Id const ) const = 0;

    virtual GridPoint      FindGridPoint( IND_ID const & ) const = 0;

    virtual PlannedActivity const & GetPlan( )  const = 0;

    virtual BYTES GetCoreSize( ) const = 0;

// point of interest

    virtual IND_ID    GetPoiId( )                const = 0;
    virtual bool      IsPoiDefined( )            const = 0; 
    virtual bool      IsPoiId( IND_ID const & )  const = 0;
    virtual GridPoint FindPOI( )                 const = 0;
    virtual bool      IsPoi  ( GridPoint const ) const = 0;

// manipulating functions

    virtual void Compute            ( ) = 0;
	virtual void ResetSelection     ( ) = 0;
    virtual void ResetAll           ( ) = 0;
    virtual void ClearPoi           ( ) = 0;
    virtual void SetPoi             ( GridPoint    const   ) = 0;
    virtual void SetSimulationMode  ( tBoolOp      const   ) = 0;
	virtual void SetSelection       ( GridRect     const & ) = 0;
    virtual void SetBrushManipulator( tManipulator const   ) = 0;
    virtual void SetBrushShape      ( tShape       const   ) = 0;
    virtual void SetBrushRadius     ( GRID_COORD   const   ) = 0;
    virtual void SetBrushIntensity  ( PERCENT      const   ) = 0;
    virtual void SetBrushMode       ( tBrushMode   const   ) = 0;
    virtual void ModelDoEdit        ( GridPoint    const   ) = 0;

    // static functions
    
    static EvolutionCore * InitClass( int const, ObserverInterface * const, EventInterface * const );
    static EvolutionCore * CreateCore( );
    
	static unsigned int GetNrInteractionsWithKnownCulprit( );
    static unsigned int GetNrInteractionsWithUnknownCulprit( );
    static unsigned int GetMaxPartnerMemory( );
	static bool         IsEnabled( Action::Id const );
};
