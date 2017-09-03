// EditorState.cpp
//

#include "stdafx.h"
#include "assert.h"
#include <unordered_map>
#include "gridRect.h"
#include "gridCircle.h"
#include "EditorState.h"

using namespace std;

EditorState::EditorState( ) :
    m_brushMode( tBrushMode::move ),
    m_brushSize( 17 ),
    m_shapeBrush( tShape::Circle ),
    m_usBrushIntensity( 50 )
{ }

void EditorState::EditorDoEdit( Grid * const pGrid, GridPoint const gp )
{
	static FoodStock_Functor  m_FoodStock_Functor;
	static Fertilizer_Functor m_Fertilizer_Functor;
	static Fertility_Functor  m_Fertility_Functor;
	static MutRate_Functor    m_MutRate_Functor;
	static Strategy_Functor   m_Strategy_Functor;

    static unordered_map < tBrushMode, GridPointIntensity_Functor * > mapFunctorTable =
    {
        { tBrushMode::move,           & m_Strategy_Functor   },
        { tBrushMode::randomStrategy, & m_Strategy_Functor   },
        { tBrushMode::cooperate,      & m_Strategy_Functor   },
        { tBrushMode::defect,         & m_Strategy_Functor   },
        { tBrushMode::tit4tat,        & m_Strategy_Functor   },
        { tBrushMode::noAnimals,      & m_Strategy_Functor   },
        { tBrushMode::mutRate,        & m_MutRate_Functor    },
        { tBrushMode::fertility,      & m_Fertility_Functor  },
        { tBrushMode::food,           & m_FoodStock_Functor  },
        { tBrushMode::fertilizer,     & m_Fertilizer_Functor }
    };

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

	GridPointIntensity_Functor * pEditFunctor = mapFunctorTable.at( m_brushMode );

    //   lint -e613  possible use of null pointer
    assert( pEditFunctor != nullptr );
	pEditFunctor->SetGrid( pGrid );
	
	if ( pEditFunctor == & m_Strategy_Functor )
		m_Strategy_Functor.SetStrategy( mapStrategyTable.at( m_brushMode ) );

	switch ( m_shapeBrush )
    {
    case tShape::Circle:
        Apply2Cone( pEditFunctor, GridCircle( gp, m_brushSize ), m_usBrushIntensity );
        break;

    case tShape::Rect:
        pEditFunctor->SetBrushIntensity( m_usBrushIntensity );
        Apply2Rect( pEditFunctor, GridRect( gp, m_brushSize ) );
        break;

    default:
        assert( false );
    }
    //   lint +e613
}

void EditorState::SetBrushStrategy( tBrushMode const brushMode )
{
    m_brushMode = brushMode;
}
