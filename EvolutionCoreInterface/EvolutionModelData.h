// EvolutionModelData.h
//

#pragma once

#include "EvolutionTypes.h"
#include "GridRect.h"

class GridPoint;
class IndId;

class EvolutionModelData
{
public:

    virtual ~EvolutionModelData( ) { };

    static EvolutionModelData * CreateModelData( );

    virtual void           CopyEvolutionModelData( EvolutionModelData const * const ) = 0;

    virtual int            GetNrOfLivingIndividuals ( ) const = 0;
    virtual int            GetAverageFoodGrowth     ( ) const = 0;
    virtual EVO_GENERATION GetEvoGenerationNr       ( ) const = 0;
    virtual short          GetBrushIntensity        ( ) const = 0;
    virtual tShape         GetBrushShape            ( ) const = 0;
    virtual GRID_COORD     GetBrushSize             ( ) const = 0;
    virtual tBrushMode     GetBrushMode             ( ) const = 0;
	virtual GridRect       GetSelection             ( ) const = 0;
	virtual bool           SelectionIsEmpty         ( ) const = 0;
	virtual bool           SelectionIsNotEmpty      ( ) const = 0;

    virtual bool           IsDead         ( GridPoint const & ) const = 0;
    virtual bool           IsAlive        ( GridPoint const & ) const = 0;
    virtual bool           IsDefined      ( GridPoint const & ) const = 0;
    virtual IndId          GetId          ( GridPoint const & ) const = 0;
    virtual tOrigin        GetOrigin      ( GridPoint const & ) const = 0;
    virtual short          GetEnergy      ( GridPoint const & ) const = 0;
    virtual int            GetFoodStock   ( GridPoint const & ) const = 0;
    virtual int            GetFertility   ( GridPoint const & ) const = 0;
    virtual int            GetMutationRate( GridPoint const & ) const = 0;
    virtual int            GetFertilizer  ( GridPoint const & ) const = 0;
    virtual EVO_GENERATION GetGenBirth    ( GridPoint const & ) const = 0;
    virtual EVO_GENERATION GetAge         ( GridPoint const & ) const = 0;
    virtual MEM_INDEX      GetMemSize     ( GridPoint const & ) const = 0;
    virtual MEM_INDEX      GetMemUsed     ( GridPoint const & ) const = 0;
    virtual tStrategyId    GetStrategyId  ( GridPoint const & ) const = 0;
    virtual long           GetMemEntry    ( GridPoint const &, MEM_INDEX const ) const = 0;
    virtual short          GetDistr       ( GridPoint const &, tAction   const ) const = 0;
    virtual long           GetGenotype    ( GridPoint const &, tGeneType const ) const = 0;

    virtual GridPoint FindGridPoint( IndId const & ) const = 0;

    // manipulating functions

	virtual void SetSelection     ( GridRect       const & ) = 0;
    virtual void SetBrushShape    ( tShape         const   ) = 0;
    virtual void SetBrushSize     ( GRID_COORD     const   ) = 0;
    virtual void SetBrushIntensity( unsigned short const   ) = 0;
    virtual void SetBrushStrategy ( tBrushMode     const   ) = 0;
    virtual void ModelDoEdit      ( GridPoint      const   ) = 0;
	virtual void ResetSelection   ( )                        = 0;
    virtual void ResetAll         ( )                        = 0;
};
