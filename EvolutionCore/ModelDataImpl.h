#pragma once

#include "ModelData.h"
#include "grid_model.h"
#include "EditorState.h"

class ModelDataImpl : public ModelData
{
public:
    ModelDataImpl( )
    {
        m_grid.ResetGrid( );
    };

    virtual void CopyModelData( ModelData const * const src )
    {
        * this = * static_cast<ModelDataImpl const *>( src );
    }

    virtual void           SetBrushShape    ( tShape     const shape ) { m_editorState.SetBrushShape    ( shape ); }
    virtual void           SetBrushSize     ( short      const iSize ) { m_editorState.SetBrushSize     ( iSize ); }
    virtual void           SetBrushIntensity( short      const iInt  ) { m_editorState.SetBrushIntensity( iInt  ); }
    virtual void           SetBrushStrategy ( tBrushMode const mode  ) { m_editorState.SetBrushStrategy ( mode  ); }

    virtual void           ModelDoEdit    ( GridPoint  const gp    ) { m_editorState.EditorDoEdit( & m_grid, gp ); }

    virtual EVO_GENERATION GetAge         ( GridPoint const & gp ) const { return m_grid.GetAge( gp ); }

    virtual int            GetFoodStock   ( GridPoint const & gp ) const { return getGridField( gp ).GetFoodStock( ); }
    virtual int            GetFertility   ( GridPoint const & gp ) const { return getGridField( gp ).GetFertility( ); }
    virtual int            GetMutationRate( GridPoint const & gp ) const { return getGridField( gp ).GetMutationRate( ); }
    virtual int            GetFertilizer  ( GridPoint const & gp ) const { return getGridField( gp ).GetFertilizer( ); }
    virtual EVO_GENERATION GetGenBirth    ( GridPoint const & gp ) const { return getGridField( gp ).GetGenBirth( ); }
    virtual IndId          GetId          ( GridPoint const & gp ) const { return getGridField( gp ).GetId( ); }
    virtual tOrigin        GetOrigin      ( GridPoint const & gp ) const { return getGridField( gp ).GetOrigin( ); }
    virtual short          GetEnergy      ( GridPoint const & gp ) const { return getGridField( gp ).GetEnergy( ); }
    virtual tStrategyId    GetStrategyId  ( GridPoint const & gp ) const { return getGridField( gp ).GetStrategyId( ); }
    virtual MEM_INDEX      GetMemSize     ( GridPoint const & gp ) const { return getGridField( gp ).GetMemSize( ); }
    virtual MEM_INDEX      GetMemUsed     ( GridPoint const & gp ) const { return getGridField( gp ).GetMemUsed( ); }
    virtual bool           IsDead         ( GridPoint const & gp ) const { return getGridField( gp ).IsDead( ); }
    virtual bool           IsAlive        ( GridPoint const & gp ) const { return getGridField( gp ).IsAlive( ); }

    virtual long           GetMemEntry    ( GridPoint const & gp, MEM_INDEX const index ) const { return getGridField( gp ).GetMemEntry( index ); }
    virtual long           GetGenotype    ( GridPoint const & gp, tGeneType const gene  ) const { return getGenome( gp ).GetAllele( gene ); }
    virtual short          GetDistr       ( GridPoint const & gp, tAction   const at    ) const { return getGenome( gp ).GetDistr( at ); }

    virtual EVO_GENERATION GetEvoGenerationNr( ) const { return m_grid.GetEvoGenerationNr( ); }
    virtual short          GetBrushIntensity ( ) const { return m_editorState.GetBrushIntensity( ); }
    virtual tShape         GetBrushShape     ( ) const { return m_editorState.GetBrushShape( ); }
    virtual short          GetBrushSize      ( ) const { return m_editorState.GetBrushSize( ); }
    virtual tBrushMode     GetBrushMode      ( ) const { return m_editorState.GetBrushMode( ); }

    virtual GridPoint      FindGridPoint( IndId const & id ) { return m_grid.FindGridPoint( id ); }

    virtual int  GetAverageFoodGrowth( )     const { return m_grid.GetAverageFoodGrowth( ); }
    virtual int  GetNrOfLivingIndividuals( ) const { return m_grid.GetNrOfLivingIndividuals( ); }

    Grid        m_grid;
    EditorState m_editorState;

private:
    GridField const & getGridField( GridPoint const & gp ) const { return m_grid.GetGridField( gp ); }
    Genome    const & getGenome   ( GridPoint const & gp ) const { return getGridField( gp ).GetGenome( ); }
};
