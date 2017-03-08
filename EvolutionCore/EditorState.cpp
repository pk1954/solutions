// EditorState.cpp
//

#include "stdafx.h"
#include <unordered_map>
#include "assert.h"
#include "gridCircle.h"
#include "EditorState.h"

using namespace std;

FoodStock_Functor  EditorState::m_FoodStock_Functor;
Fertilizer_Functor EditorState::m_Fertilizer_Functor;
Fertility_Functor  EditorState::m_Fertility_Functor;
MutRate_Functor    EditorState::m_MutRate_Functor;
Strategy_Functor   EditorState::m_Strategy_Functor;

EditorState::EditorState( ) :
    m_brushMode(  tBrushMode::move ),
    m_sBrushSize( 17 ),
    m_shapeBrush( tShape::Circle ),
    m_sBrushIntensity( 50 )
{ }

void EditorState::InitEditorState( Grid * const pGrid )
{
    m_Fertilizer_Functor.SetGrid( pGrid );
    m_FoodStock_Functor.SetGrid( pGrid );
    m_Fertility_Functor.SetGrid( pGrid );
    m_MutRate_Functor.SetGrid( pGrid );
    m_Strategy_Functor.SetGrid( pGrid );
}

void EditorState::DoEdit( GridPoint const gp )
{
    static unordered_map < tBrushMode, GridPointIntensity_Functor * > mapFunctorTable =
    {
        {  tBrushMode::move,           & m_Strategy_Functor   },
        {  tBrushMode::randomStrategy, & m_Strategy_Functor   },
        {  tBrushMode::cooperate,      & m_Strategy_Functor   },
        {  tBrushMode::defect,         & m_Strategy_Functor   },
        {  tBrushMode::tit4tat,        & m_Strategy_Functor   },
        {  tBrushMode::noAnimals,      & m_Strategy_Functor   },
        {  tBrushMode::mutRate,        & m_MutRate_Functor    },
        {  tBrushMode::fertility,      & m_Fertility_Functor  },
        {  tBrushMode::food,           & m_FoodStock_Functor  },
        {  tBrushMode::fertilizer,     & m_Fertilizer_Functor }
    };

    GridPointIntensity_Functor * pEditFunctor = mapFunctorTable.at( m_brushMode );

    //   lint -e613  possible use of null pointer
    assert( pEditFunctor != nullptr );
    switch ( m_shapeBrush )
    {
    case tShape::Circle:
        Apply2Cone( pEditFunctor, GridCircle( gp, m_sBrushSize ), m_sBrushIntensity );
        break;

    case tShape::Rect:
        pEditFunctor->SetBrushIntensity( m_sBrushIntensity );
        Apply2Rect( pEditFunctor, GridRect( gp, m_sBrushSize ) );
        break;

    default:
        assert( false );
    }
    //   lint +e613
}

void EditorState::SetBrushStrategy( tBrushMode const brushMode )
{
    static unordered_map < tBrushMode, tStrategyId > mapStrategyTable =
    {
        { tBrushMode::randomStrategy, tStrategyId::random },
        { tBrushMode::cooperate,      tStrategyId::cooperateAlways },
        { tBrushMode::defect,         tStrategyId::defectAlways },
        { tBrushMode::tit4tat,        tStrategyId::tit4tat },
        { tBrushMode::noAnimals,      tStrategyId::empty },
        { tBrushMode::move,           tStrategyId::empty },
        { tBrushMode::mutRate,        tStrategyId::empty },
        { tBrushMode::fertility,      tStrategyId::empty },
        { tBrushMode::food,           tStrategyId::empty },
        { tBrushMode::fertilizer,     tStrategyId::empty }
    };

    m_brushMode = brushMode;
    m_Strategy_Functor.SetStrategy( mapStrategyTable.at( brushMode ) );
}
