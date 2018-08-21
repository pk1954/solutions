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

Set_Manipulator <short> * EditorState::m_setMan; 
Add_Manipulator <short> * EditorState::m_addMan; 
Subt_Manipulator<short> * EditorState::m_subMan;    
Max_Manipulator <short> * EditorState::m_maxMan; 
Min_Manipulator <short> * EditorState::m_minMan; 
Mean_Manipulator<short> * EditorState::m_meanMan;

EditorState::EditorState( )
{ 
	m_setMan  = new Set_Manipulator <short>;
	m_addMan  = new Add_Manipulator <short>;   
	m_subMan  = new Subt_Manipulator<short>;   
	m_maxMan  = new Max_Manipulator <short>;   
	m_minMan  = new Min_Manipulator <short>;   
	m_meanMan = new Mean_Manipulator<short>;  

	Reset( );
}

void EditorState::Reset( )
{
	SetBrushOperator( tOperator::add );
    m_brushMode      = tBrushMode::move;
    m_brushShape     = tShape::Circle;
    m_brushSize      = 17;
    m_brushIntensity = 50;
}

void EditorState::SetBrushOperator( tOperator  const op ) 
{ 
	switch ( op )
	{
	case tOperator::set      : m_manipulator = m_setMan;  break; 
	case tOperator::add      : m_manipulator = m_addMan;  break; 
	case tOperator::subtract : m_manipulator = m_subMan;  break; 
	case tOperator::max      : m_manipulator = m_maxMan;  break; 
	case tOperator::min      : m_manipulator = m_minMan;  break; 
	case tOperator::mean     : m_manipulator = m_meanMan; break; 
	}
}
  
void EditorState::EditorDoEdit( Grid & G, GridPoint const gpCenter )
{
	GridPointFuncShort lambdaMode;

	switch ( m_brushMode )
    {
	case  tBrushMode::move:        lambdaMode = [&G, this](GridPoint const & gp, short const s) { G.EditSetStrategy(gp, s, tStrategyId::empty    ); }; break;
	case  tBrushMode::randomStrat: lambdaMode = [&G, this](GridPoint const & gp, short const s) { G.EditSetStrategy(gp, s, tStrategyId::random   ); }; break;
    case  tBrushMode::cooperate:   lambdaMode = [&G, this](GridPoint const & gp, short const s) { G.EditSetStrategy(gp, s, tStrategyId::cooperate); }; break;
    case  tBrushMode::defect:      lambdaMode = [&G, this](GridPoint const & gp, short const s) { G.EditSetStrategy(gp, s, tStrategyId::defect   ); }; break;
    case  tBrushMode::tit4tat:     lambdaMode = [&G, this](GridPoint const & gp, short const s) { G.EditSetStrategy(gp, s, tStrategyId::tit4tat  ); }; break;
    case  tBrushMode::noAnimals:   lambdaMode = [&G, this](GridPoint const & gp, short const s) { G.EditSetStrategy(gp, s, tStrategyId::empty    ); }; break;
    case  tBrushMode::mutRate:     lambdaMode = [&G, this](GridPoint const & gp, short const s) { G.Apply2MutRate   (gp, s, m_manipulator); }; break;
    case  tBrushMode::fertility:   lambdaMode = [&G, this](GridPoint const & gp, short const s) { G.Apply2Fertility (gp, s, m_manipulator); }; break;
    case  tBrushMode::food:        lambdaMode = [&G, this](GridPoint const & gp, short const s) { G.Apply2FoodStock (gp, s, m_manipulator); }; break;
    case  tBrushMode::fertilizer:  lambdaMode = [&G, this](GridPoint const & gp, short const s) { G.Apply2Fertilizer(gp, s, m_manipulator); }; break;
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
