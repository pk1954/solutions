// EvolutionDataModelImpl.h
//

#pragma once

#include <functional>
#include "boolOp.h"
#include "EvolutionModelData.h"
#include "gridRect.h"
#include "grid_model.h"
#include "EditorState.h"

class EvolutionModelDataImpl : public EvolutionModelData
{
public:
    EvolutionModelDataImpl( ) : 
		m_editorState    ( & m_grid ),
		m_editorStateLast( & m_grid )
    {
        ResetAll( );
    };

	void ResetAll( )
	{
        m_grid.ResetGrid( );
	    m_idPOI.ResetIndId( );
		m_editorState.Reset( );
	    m_plan.SetInvalid( );
		ResetSelection( );
        SetBrushMode( tBrushMode::move );
	}

    virtual void CopyEvolutionModelData( EvolutionModelData const * const );

	virtual void           ResetSelection   ( )                       { m_gridRectSelection.Reset(); };
	virtual void           SetSelection     ( GridRect const & rect ) { m_gridRectSelection = rect; }

    virtual void           SetBrushOperator ( tOperator  const op    ) { m_editorState.SetBrushOperator ( op    ); }
    virtual void           SetBrushShape    ( tShape     const shape ) { m_editorState.SetBrushShape    ( shape ); }
    virtual void           SetBrushRadius   ( GRID_COORD const r     ) { m_editorState.SetBrushRadius   ( r  ); }
    virtual void           SetBrushIntensity( short      const sInt  ) { m_editorState.SetBrushIntensity( sInt  ); }
    virtual void           SetSimulationMode( tBoolOp    const op    ) { m_editorState.SetSimulationMode( op    ); }

    virtual void           SetBrushMode     ( tBrushMode const mode  ) { m_editorState.SetBrushMode( mode ); }
    virtual void           ModelDoEdit      ( GridPoint  const gp )    { m_editorState.EditorDoEdit( gp ); }

	virtual bool           GetSimulationMode( ) const { return m_editorState.GetSimulationMode( ); }

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
    virtual bool           IsDefined      ( GridPoint const & gp ) const { return getGridField( gp ).IsDefined( ); }

    virtual long           GetMemEntry    ( GridPoint const & gp, MEM_INDEX const index ) const { return getGridField( gp ).GetMemEntry( index ); }
    virtual long           GetGenotype    ( GridPoint const & gp, tGeneType const gene  ) const { return getGenome( gp ).GetAllele( gene ); }
    virtual short          GetDistr       ( GridPoint const & gp, tAction   const at    ) const { return getGenome( gp ).GetDistr( at ); }

    virtual EVO_GENERATION GetEvoGenerationNr ( ) const { return m_grid.GetEvoGenerationNr( ); }
    virtual short          GetBrushIntensity  ( ) const { return m_editorState.GetBrushIntensity( ); }
    virtual tOperator      GetBrushOperator   ( ) const { return m_editorState.GetBrushOperator( ); }
    virtual tShape         GetBrushShape      ( ) const { return m_editorState.GetBrushShape( ); }
    virtual GRID_COORD     GetBrushSize       ( ) const { return m_editorState.GetBrushSize( ); }
    virtual tBrushMode     GetBrushMode       ( ) const { return m_editorState.GetBrushMode( ); }
	virtual GridRect       GetSelection       ( ) const { return m_gridRectSelection.IsEmpty( ) ? GridRect::GRID_RECT_FULL : m_gridRectSelection; }
	virtual bool           SelectionIsEmpty   ( ) const { return m_gridRectSelection.IsEmpty(); }
	virtual bool           SelectionIsNotEmpty( ) const { return m_gridRectSelection.IsNotEmpty(); }

	virtual IndId     GetPoiId    ( )                      const { return m_idPOI; }
	virtual bool      IsPoiDefined( )                      const { return m_idPOI.IsDefined( ); }
	virtual bool      IsPoiId     ( IndId     const & id ) const { return m_idPOI == id; }
	virtual bool      IsPoi       ( GridPoint const & gp ) const { return ( gp.IsNotNull( ) && ( GetId( gp ) == m_idPOI ) ); }
	virtual void      ClearPoi    ( )                            { m_idPOI.ResetIndId( ); }

	virtual void      SetPoi( GridPoint const & );     
    virtual GridPoint FindPOI( ) const;
    virtual GridPoint FindGridPoint( IndId const & ) const;

	virtual void SaveEditorState      ( )       {        m_editorStateLast  = m_editorState; }
	virtual bool EditorStateHasChanged( ) const { return m_editorStateLast != m_editorState; }

    virtual PlannedActivity const & GetPlan( )         const { return   m_plan; };
    virtual PlannedActivity       * GetPlan4Writing( )       { return & m_plan; };

    virtual int GetAverageFoodGrowth( )     const { return m_grid.GetAverageFoodGrowth( ); }
    virtual int GetNrOfLivingIndividuals( ) const { return m_grid.GetNrOfLivingIndividuals( ); }

    Grid            m_grid;	

private:
    PlannedActivity m_plan;
    IndId           m_idPOI;
    EditorState     m_editorState;
	EditorState     m_editorStateLast;
	GridRect        m_gridRectSelection;

    GridField const & getGridField( GridPoint const & gp ) const { return m_grid.GetGridField( gp ); }
    Genome    const & getGenome   ( GridPoint const & gp ) const { return getGridField( gp ).GetGenome( ); }
};
