// EditorState.cpp
//

#include "stdafx.h"
#include "assert.h"
#include <unordered_map>
#include "gridRect.h"
#include "gridCircle.h"
#include "grid_model.h"
#include "EditorState.h"

using namespace std;

EditorState::EditorState( )
{ 
	Reset( );
}

void EditorState::Reset( )
{
    m_brushMode = tBrushMode::move;
    m_brushSize = 17;
    m_brushShape = tShape::Circle;
    m_brushIntensity= 50;
}

void EditorState::EditorDoEdit( Grid * const pGrid, GridPoint const gp )
{
	std::function<void(GridPoint const &, short const)> lambda;

	switch ( m_brushMode )
    {
	case  tBrushMode::move:           lambda = [&](GridPoint const & gp, short const s) { pGrid->EditSetStrategy(gp, s, tStrategyId::empty          ); }; break;
	case  tBrushMode::randomStrategy: lambda = [&](GridPoint const & gp, short const s) { pGrid->EditSetStrategy(gp, s, tStrategyId::random         ); }; break;
    case  tBrushMode::cooperate:      lambda = [&](GridPoint const & gp, short const s) { pGrid->EditSetStrategy(gp, s, tStrategyId::cooperateAlways); }; break;
    case  tBrushMode::defect:         lambda = [&](GridPoint const & gp, short const s) { pGrid->EditSetStrategy(gp, s, tStrategyId::defectAlways   ); }; break;
    case  tBrushMode::tit4tat:        lambda = [&](GridPoint const & gp, short const s) { pGrid->EditSetStrategy(gp, s, tStrategyId::tit4tat        ); }; break;
    case  tBrushMode::noAnimals:      lambda = [&](GridPoint const & gp, short const s) { pGrid->EditSetStrategy(gp, s, tStrategyId::empty          ); }; break;
    case  tBrushMode::mutRate:        lambda = [&](GridPoint const & gp, short const s) { pGrid->IncMutationRate(gp, s); }; break;
    case  tBrushMode::fertility:      lambda = [&](GridPoint const & gp, short const s) { pGrid->IncFertility   (gp, s); }; break;
    case  tBrushMode::food:           lambda = [&](GridPoint const & gp, short const s) { pGrid->IncFoodStock   (gp, s); }; break;
    case  tBrushMode::fertilizer:     lambda = [&](GridPoint const & gp, short const s) { pGrid->IncFertilizer  (gp, s); }; break;
	};

	switch ( m_brushShape )
    {
    case tShape::Circle:
        GridCircle( gp, m_brushSize ).Apply2Cone( lambda, m_brushIntensity );
        break;

    case tShape::Rect:
		GridRect( gp, m_brushSize ).Apply2Rect( lambda, m_brushIntensity  );        
		break;

    default:
        assert( false );
    }
}
