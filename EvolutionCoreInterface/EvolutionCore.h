// EvolutionCore.h
//

#pragma once

#include "EvolutionTypes.h"

class PlannedActivity;
class GridPoint;
class IndId;
class DisplayFunctor;
class EvolutionModelData;

class EvolutionCore
{
public:
    virtual ~EvolutionCore( ) { };

    virtual PlannedActivity const & GetPlan( ) const = 0;

// point of interest

    virtual IndId     GetPoiId( )                                              const = 0;
    virtual bool      IsPoiDefined( )                                          const = 0; 
    virtual bool      IsPoiId( IndId                const & )                  const = 0;
    virtual GridPoint FindPOI( EvolutionModelData * const )                    const = 0;
    virtual bool      IsPoi  ( EvolutionModelData * const, GridPoint const & ) const = 0;
    virtual void      SetPoi ( EvolutionModelData * const, GridPoint const & )       = 0;
    virtual void      ClearPoi( )                                                    = 0;

// debugging functions

    virtual void DumpGridPointList( ) const = 0;

// manipulating functions

    virtual void SetGridDisplayFunctor( DisplayFunctor const * const ) = 0;

    virtual void ResetModel( EvolutionModelData * const ) = 0;
    virtual void Compute   ( EvolutionModelData * const ) = 0;

    virtual void SaveEditorState      ( EvolutionModelData * const )       = 0;
    virtual bool EditorStateHasChanged( EvolutionModelData * const ) const = 0;

    // static functions
    
    static void            InitClass( );
    static EvolutionCore * CreateCore( );
    static void            DeleteCore( );
    static unsigned long   GetModelSize( );
    static unsigned int    GetNrInteractionsWithKnownCulprit( );
    static unsigned int    GetNrInteractionsWithUnknownCulprit( );
    static unsigned int    GetMaxPartnerMemory( );
    static int             GetStdCapacity( );
};
