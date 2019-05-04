// gridBrush.cpp
//
// EvolutionCore

#include "stdafx.h"
#include "grid_model.h"
#include "gridBrush.h"

using namespace std;
	
GridBrush::GridBrush( Grid * const pGrid ) :
	m_pGrid( pGrid ),
	m_shape( tShape::undefined ),
	m_brushMode( tBrushMode::undefined ),
	m_manipulator( tManipulator::undefined ),
	m_intensity( 0_PERCENT ),
	m_radius( 0_GRID_COORD ),
	m_manFunc( nullptr )
{
	Reset();
}

void GridBrush::Reset( )
{
	SetBrushMode( tBrushMode::move );
	SetManipulator( tManipulator::add );
	SetShape( tShape::Circle );
	SetRadius( 17_GRID_COORD );
	SetIntensity( 50_PERCENT );
}

void GridBrush::SetBrushMode( tBrushMode const mode ) 
{ 
	m_brushMode = mode;  
	switch ( m_brushMode )
	{
	case  tBrushMode::move:        m_func = [this](GridPoint const gp, short const s) { m_pGrid->EditSetStrategy (gp, PERCENT(s), Strategy::Id::empty    ); }; break;
	case  tBrushMode::randomStrat: m_func = [this](GridPoint const gp, short const s) { m_pGrid->EditSetStrategy (gp, PERCENT(s), Strategy::Id::random   ); }; break;
	case  tBrushMode::cooperate:   m_func = [this](GridPoint const gp, short const s) { m_pGrid->EditSetStrategy (gp, PERCENT(s), Strategy::Id::cooperate); }; break;
	case  tBrushMode::defect:      m_func = [this](GridPoint const gp, short const s) { m_pGrid->EditSetStrategy (gp, PERCENT(s), Strategy::Id::defect   ); }; break;
	case  tBrushMode::tit4tat:     m_func = [this](GridPoint const gp, short const s) { m_pGrid->EditSetStrategy (gp, PERCENT(s), Strategy::Id::tit4tat  ); }; break;
	case  tBrushMode::noAnimals:   m_func = [this](GridPoint const gp, short const s) { m_pGrid->EditSetStrategy (gp, PERCENT(s), Strategy::Id::empty    ); }; break;
	case  tBrushMode::mutRate:     m_func = [this](GridPoint const gp, short const s) { m_pGrid->Apply2MutRate   (gp, PERCENT(s),      m_manFunc); }; break;
	case  tBrushMode::fertilizer:  m_func = [this](GridPoint const gp, short const s) { m_pGrid->Apply2Fertilizer(gp, ENERGY_UNITS(s), m_manFunc); }; break;
	case  tBrushMode::fertility:   m_func = [this](GridPoint const gp, short const s) { m_pGrid->Apply2Fertility (gp, ENERGY_UNITS(s), m_manFunc); }; break;
	case  tBrushMode::food:        m_func = [this](GridPoint const gp, short const s) { m_pGrid->Apply2FoodStock (gp, ENERGY_UNITS(s), m_manFunc); }; break;
	};
}

void GridBrush::SetManipulator( tManipulator const man ) 
{ 
	m_manipulator = man;
	switch ( man )
	{
	case tManipulator::set      : m_manFunc = [](short const dst, short const src) { return src;                    };  break; 
	case tManipulator::add      : m_manFunc = [](short const dst, short const src) { return (dst + src);            };  break; 
	case tManipulator::subtract : m_manFunc = [](short const dst, short const src) { return std::max(0, dst - src); };  break; 
	case tManipulator::max      : m_manFunc = [](short const dst, short const src) { return std::max(dst, src);     };  break;
	case tManipulator::min      : m_manFunc = [](short const dst, short const src) { return std::min(dst, src);     };  break; 
	default: assert( false );
	}
}

void GridBrush::SetShape( tShape const shape )
{
	m_shape = shape;
	switch ( m_shape )
	{
	case tShape::Circle:
		m_filter = [this]( GridPoint const gp )
		{ 
			long    const lRadius     { m_radius.GetValue() };
			long    const lRadSquare  { lRadius * lRadius };
			long    const lx          { gp.GetXvalue() };
			long    const ly          { gp.GetYvalue() };
			long    const lDistSquare { lx * lx + ly * ly  };
			long    const lReduction  { (m_intensity.GetValue() * lDistSquare) / lRadSquare };
			PERCENT const reduction   { CastToShort( lReduction ) };
			return m_intensity - reduction;
		};
		break;

	case tShape::Grid:
	case tShape::Rect:
		m_filter = [this]( GridPoint const gp )
		{ 
			return m_intensity;
		};
		break;

	default:
		assert( false );
	}
}

void GridBrush::SetRadius( GRID_COORD const radius ) 
{ 
	assert( radius <= MAX_GRID_COORD );
	m_radius = radius;    
}

void GridBrush::SetIntensity( PERCENT const intensity ) 
{ 
	m_intensity = intensity; 
}

void GridBrush::operator()( GridPoint gpCenter )
{
	if (m_shape == tShape::Grid)
	{
		Apply2Grid
		( 
			[&](GridPoint const gp) 
			{ 
				(m_func)( gp, m_intensity.GetValue() ); 
				return false;
			} 
		);
	}
	else
	{
		Apply2Rect
		(
			[&](GridPoint const gp)
			{
				PERCENT intensity = m_filter( gp );
				if ( intensity >= 0_PERCENT )
					(m_func)( gp + gpCenter, intensity.GetValue() );
				return false;
			},
			ClipToGrid( gpCenter - GridPoint( m_radius ) ) - gpCenter,
			ClipToGrid( gpCenter + GridPoint( m_radius ) ) - gpCenter
		);
	}
}

bool GridBrush::operator!=( GridBrush const & other ) const
{
	return ( m_manipulator != other.m_manipulator )
		|| ( m_brushMode   != other.m_brushMode )
		|| ( m_radius      != other.m_radius )      
		|| ( m_intensity   != other.m_intensity )
		|| ( m_shape       != other.m_shape );
}
