// EditorState.cpp
//

#include "stdafx.h"
#include "assert.h"
#include <unordered_map>
#include "gridPoint.h"
#include "gridShape.h"
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
	m_lambdaMode     = nullptr;
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
  
void EditorState::SetBrushMode( Grid & G, tBrushMode const mode  ) 
{ 
	m_brushMode = mode;  

	switch ( m_brushMode )
	{
	case  tBrushMode::move:        m_lambdaMode = [&G, this](GridPoint const & gp, short const s) { G.EditSetStrategy (gp, s, m_manipulator, tStrategyId::empty    ); }; break;
	case  tBrushMode::randomStrat: m_lambdaMode = [&G, this](GridPoint const & gp, short const s) { G.EditSetStrategy (gp, s, m_manipulator, tStrategyId::random   ); }; break;
	case  tBrushMode::cooperate:   m_lambdaMode = [&G, this](GridPoint const & gp, short const s) { G.EditSetStrategy (gp, s, m_manipulator, tStrategyId::cooperate); }; break;
	case  tBrushMode::defect:      m_lambdaMode = [&G, this](GridPoint const & gp, short const s) { G.EditSetStrategy (gp, s, m_manipulator, tStrategyId::defect   ); }; break;
	case  tBrushMode::tit4tat:     m_lambdaMode = [&G, this](GridPoint const & gp, short const s) { G.EditSetStrategy (gp, s, m_manipulator, tStrategyId::tit4tat  ); }; break;
	case  tBrushMode::noAnimals:   m_lambdaMode = [&G, this](GridPoint const & gp, short const s) { G.EditSetStrategy (gp, s, m_manipulator, tStrategyId::empty    ); }; break;
	case  tBrushMode::mutRate:     m_lambdaMode = [&G, this](GridPoint const & gp, short const s) { G.Apply2MutRate   (gp, s, m_manipulator); }; break;
	case  tBrushMode::fertility:   m_lambdaMode = [&G, this](GridPoint const & gp, short const s) { G.Apply2Fertility (gp, s, m_manipulator); }; break;
	case  tBrushMode::food:        m_lambdaMode = [&G, this](GridPoint const & gp, short const s) { G.Apply2FoodStock (gp, s, m_manipulator); }; break;
	case  tBrushMode::fertilizer:  m_lambdaMode = [&G, this](GridPoint const & gp, short const s) { G.Apply2Fertilizer(gp, s, m_manipulator); }; break;
	};
}
  
void EditorState::EditorDoEdit( GridPoint const gpCenter )
{
	switch ( m_brushShape )
    {
    case tShape::Circle:
        GridCircle( gpCenter, m_brushSize ).Apply2Shape( m_lambdaMode, m_brushIntensity );
        break;

    case tShape::Rect:
		GridRect( gpCenter, m_brushSize ).Apply2Shape( m_lambdaMode, m_brushIntensity ); 
		break;

    default:
        assert( false );
    }
}
