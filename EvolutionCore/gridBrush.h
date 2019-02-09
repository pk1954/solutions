// gridBrush.h
//

#pragma once

#include <algorithm>  // min/max templates
#include "gridPoint.h"
#include "EvolutionTypes.h"
#include "grid_model.h"

class GridBrush
{
public:
	GridBrush( Grid * const pGrid )
		: m_pGrid( pGrid )
	{
		Reset();
	}

	virtual ~GridBrush() {};

	void Reset( )
	{
		SetBrushMode( tBrushMode::move );
		SetManipulator( tManipulator::add );
		SetShape( tShape::Circle );
		SetRadius( GRID_COORD(17_GRID_COORD) );
		SetIntensity( 50 );
	}

	virtual void operator()( GridPoint gpCenter )
	{
		if (m_shape == tShape::Grid)
		{
			Apply2Grid( [&](GridPoint const gp) { (m_func)( gp, m_sIntensity ); } );
		}
		else
		{
			Apply2Rect
			(
				[&](GridPoint const gp)
				{
					short sIntensity = m_filter( gp );
					if ( sIntensity >= 0)
						(m_func)( gp + gpCenter, sIntensity );
				},
				Max( GRID_ORIGIN()  - gpCenter, GridPoint( -m_radius, -m_radius ) ),
				Min( GRID_MAXIMUM() - gpCenter, GridPoint(  m_radius,  m_radius ) )
			);
		}
	}

	void SetRadius( GRID_COORD const radius ) 
	{ 
		assert( radius <= MAX_GRID_COORD );
		m_radius = radius;    
	}

	void SetIntensity( short const intensity ) 
	{ 
		m_sIntensity = intensity; 
	}

	void SetBrushMode( tBrushMode const mode ) 
	{ 
		m_brushMode = mode;  
		switch ( m_brushMode )
		{
		case  tBrushMode::move:        m_func = [this](GridPoint const gp, short const s) { m_pGrid->EditSetStrategy (gp, s, tStrategyId::empty    ); }; break;
		case  tBrushMode::randomStrat: m_func = [this](GridPoint const gp, short const s) { m_pGrid->EditSetStrategy (gp, s, tStrategyId::random   ); }; break;
		case  tBrushMode::cooperate:   m_func = [this](GridPoint const gp, short const s) { m_pGrid->EditSetStrategy (gp, s, tStrategyId::cooperate); }; break;
		case  tBrushMode::defect:      m_func = [this](GridPoint const gp, short const s) { m_pGrid->EditSetStrategy (gp, s, tStrategyId::defect   ); }; break;
		case  tBrushMode::tit4tat:     m_func = [this](GridPoint const gp, short const s) { m_pGrid->EditSetStrategy (gp, s, tStrategyId::tit4tat  ); }; break;
		case  tBrushMode::noAnimals:   m_func = [this](GridPoint const gp, short const s) { m_pGrid->EditSetStrategy (gp, s, tStrategyId::empty    ); }; break;
		case  tBrushMode::mutRate:     m_func = [this](GridPoint const gp, short const s) { m_pGrid->Apply2MutRate   (gp, s, m_manFunc); }; break;
		case  tBrushMode::fertility:   m_func = [this](GridPoint const gp, short const s) { m_pGrid->Apply2Fertility (gp, s, m_manFunc); }; break;
		case  tBrushMode::food:        m_func = [this](GridPoint const gp, short const s) { m_pGrid->Apply2FoodStock (gp, s, m_manFunc); }; break;
		case  tBrushMode::fertilizer:  m_func = [this](GridPoint const gp, short const s) { m_pGrid->Apply2Fertilizer(gp, s, m_manFunc); }; break;
		};
	}

	void SetManipulator( tManipulator const man ) 
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
  
	void SetShape( tShape const shape )
	{
		m_shape = shape;
		switch ( m_shape )
		{
		case tShape::Circle:
			m_filter = [this]( GridPoint const gp )
			{ 
				long  const lRadius     = m_radius.GetValue();
				long  const lRadSquare  = lRadius * lRadius;
				long  const lx          = gp.GetXvalue();
				long  const ly          = gp.GetYvalue();
				long  const lDistSquare = lx * lx + ly * ly;
				short const sReduce     = CastToShort(( m_sIntensity * lDistSquare) / lRadSquare);
				return m_sIntensity - sReduce;
			};
			break;

		case tShape::Grid:
		case tShape::Rect:
			m_filter = [this]( GridPoint const gp )
			{ 
				return m_sIntensity;
			};
			break;

		default:
			assert( false );
		}
	}

    bool operator!=( GridBrush const & other ) const
    {
        return ( m_manipulator != other.m_manipulator )
            || ( m_brushMode   != other.m_brushMode )
            || ( m_radius      != other.m_radius )      
			|| ( m_sIntensity  != other.m_sIntensity )
            || ( m_shape       != other.m_shape );
    }
 
    GRID_COORD   const GetRadius     ( ) const { return m_radius;      }
	short        const GetIntensity  ( ) const { return m_sIntensity;  }
	tShape       const GetShape      ( ) const { return m_shape;       }
    tBrushMode   const GetBrushMode  ( ) const { return m_brushMode;   }
    tManipulator const GetManipulator( ) const { return m_manipulator; }

private:
	
	Grid            * m_pGrid;

	tShape			  m_shape;
    tBrushMode		  m_brushMode;
	tManipulator	  m_manipulator;
	short             m_sIntensity;
    GRID_COORD	      m_radius;

	ManipulatorFunc   m_manFunc;

	std::function<void (GridPoint const, short const)> m_func;
    std::function<short(GridPoint const)>              m_filter;
};
