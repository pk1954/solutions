// EditorState.cpp
//

#include "stdafx.h"
#include "assert.h"
#include <unordered_map>
#include "gridPoint.h"
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

void EditorState::EditorDoEdit( Grid & G, GridPoint const gpCenter )
{
	GridPointFuncShort lambdaMode;

	switch ( m_brushMode )
    {
	case  tBrushMode::move:           lambdaMode = [&G](GridPoint const & gp, short const s) { G.EditSetStrategy(gp, s, tStrategyId::empty          ); }; break;
	case  tBrushMode::randomStrategy: lambdaMode = [&G](GridPoint const & gp, short const s) { G.EditSetStrategy(gp, s, tStrategyId::random         ); }; break;
    case  tBrushMode::cooperate:      lambdaMode = [&G](GridPoint const & gp, short const s) { G.EditSetStrategy(gp, s, tStrategyId::cooperateAlways); }; break;
    case  tBrushMode::defect:         lambdaMode = [&G](GridPoint const & gp, short const s) { G.EditSetStrategy(gp, s, tStrategyId::defectAlways   ); }; break;
    case  tBrushMode::tit4tat:        lambdaMode = [&G](GridPoint const & gp, short const s) { G.EditSetStrategy(gp, s, tStrategyId::tit4tat        ); }; break;
    case  tBrushMode::noAnimals:      lambdaMode = [&G](GridPoint const & gp, short const s) { G.EditSetStrategy(gp, s, tStrategyId::empty          ); }; break;
    case  tBrushMode::mutRate:        lambdaMode = [&G](GridPoint const & gp, short const s) { G.IncMutationRate(gp, s); }; break;
    case  tBrushMode::fertility:      lambdaMode = [&G](GridPoint const & gp, short const s) { G.IncFertility   (gp, s); }; break;
    case  tBrushMode::food:           lambdaMode = [&G](GridPoint const & gp, short const s) { G.IncFoodStock   (gp, s); }; break;
    case  tBrushMode::fertilizer:     lambdaMode = [&G](GridPoint const & gp, short const s) { G.IncFertilizer  (gp, s); }; break;
	};

	switch ( m_brushShape )
    {
    case tShape::Circle:
        GridCircle( gpCenter, m_brushSize ).Apply2Cone( lambdaMode, m_brushIntensity );
        break;

    case tShape::Rect:
		GridRect( gpCenter, m_brushSize ).Apply2Rect( lambdaMode, m_brushIntensity  );        
		break;

    default:
        assert( false );
    }
}
