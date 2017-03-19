#pragma once

#include "ModelDataImpl.h"
#include "EvolutionCore.h"
#include "EditorState.h"
#include "gridFunctors.h"

class EvolutionCoreImpl : public EvolutionCore
{
public:
    explicit EvolutionCoreImpl( );
    ~EvolutionCoreImpl( );

    virtual void      ResetModel           ( EvolutionModelData * const );
    virtual void      Compute              ( EvolutionModelData * const );
    virtual void      SetGridDisplayFunctor( DisplayFunctor const * const f ) { m_gridDisplayFunctor = f; }

    virtual void      SaveEditorState      ( EvolutionModelData * const pModel )       {        m_editorStateLast  = static_cast< ModelDataImpl * >( pModel )->m_editorState; }
    virtual bool      EditorStateHasChanged( EvolutionModelData * const pModel ) const { return m_editorStateLast != static_cast< ModelDataImpl * >( pModel )->m_editorState; }

    virtual GridPoint FindPOI( EvolutionModelData * const pModel )                       const { return IsPoiDefined( ) ? static_cast< ModelDataImpl * >( pModel )->m_grid.FindGridPoint( m_idPOI ) : GridPoint::GP_NULL; }
    virtual IndId     GetPoiId( )                                               const { return m_idPOI; }
    virtual bool      IsPoiDefined( )                                           const { return m_idPOI.IsDefined( ); }
    virtual bool      IsPoiId( IndId       const & id )                         const { return m_idPOI == id; }
    virtual bool      IsPoi  ( EvolutionModelData * const pModel, GridPoint const & gp ) const { return ( gp.IsNotNull( ) && ( pModel->GetId( gp ) == m_idPOI ) ); }
    virtual void      SetPoi ( EvolutionModelData * const pModel, GridPoint const & gp )       { m_idPOI = static_cast< ModelDataImpl * >( pModel )->m_grid.GetId( gp ); }
    virtual void      ClearPoi( )                                                     { m_idPOI.ResetIndId( ); }

    virtual PlannedActivity const & GetPlan( ) const { return m_plan; };

    virtual void DumpGridPointList( ) const;

    static  int  GetStdCapacity( ) { return m_iStdCapacity; };

private:
    PlannedActivity        m_plan;
    IndId                  m_idPOI;
    DisplayFunctor const * m_gridDisplayFunctor;    // GUI call back for display of current model 
    EditorState            m_editorStateLast;

    // private member functions

    EvolutionCoreImpl( EvolutionCoreImpl const & );               // noncopyable class 
    EvolutionCoreImpl & operator= ( EvolutionCoreImpl const & );  // noncopyable class 

    void dumpGridPointList( Grid const & ) const;

    // static members

    static FoodStock_Functor  m_FoodStock_Functor;
    static Fertilizer_Functor m_Fertilizer_Functor;
    static Fertility_Functor  m_Fertility_Functor;
    static MutRate_Functor    m_MutRate_Functor;
    static Strategy_Functor   m_Strategy_Functor;

    static int m_iStdCapacity;  // cached config data
};
