// gridBrush.h
//

#pragma once

#include <algorithm>  // min/max templates
#include "gridPoint.h"
#include "Manipulator.h"
#include "EvolutionTypes.h"
#include "grid_model.h"

class GridBrush
{
public:
	GridBrush( Grid * const pGrid )
		: m_pGrid( pGrid )
	{
		if ( ! m_bClassInitialized )
		{
			m_setMan  = new Set_Manipulator <short>;
			m_addMan  = new Add_Manipulator <short>;   
			m_subMan  = new Subt_Manipulator<short>;   
			m_maxMan  = new Max_Manipulator <short>;   
			m_minMan  = new Min_Manipulator <short>;   
			m_meanMan = new Mean_Manipulator<short>;  
		}

		Reset();
	}

	virtual ~GridBrush() {};

	void Reset( )
	{
		m_gpCenter = GridPoint::GP_NULL;
		m_func = nullptr;
		SetOperator( tOperator::add );
		SetShape( tShape::Circle );
		SetRadius( 17 );
		SetIntensity( 50 );
	}

	virtual void operator()( GridPoint const gpCenter )
	{
		m_gpCenter = gpCenter;

		GridPoint gpStart = Max( GridPoint::GRID_ORIGIN  - gpCenter, GridPoint::GRID_ORIGIN - m_radius );
		GridPoint gpEnd   = Min( GridPoint::GRID_MAXIMUM - gpCenter, GridPoint::GRID_ORIGIN + m_radius );

		GridPoint gp;

		for ( gp.y = gpStart.y; gp.y <= m_radius; ++gp.y )
			for ( gp.x = gpStart.x; gp.x <= m_radius; ++gp.x )
				m_filter( gp );
	}

	bool const IsEmpty( )    const { return ( m_gpCenter == GridPoint::GP_NULL ); }
    bool const IsNotEmpty( ) const { return ( m_gpCenter != GridPoint::GP_NULL ); }

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
		case  tBrushMode::move:        m_func = [this](GridPoint const & gp, short const s) { m_pGrid->EditSetStrategy (gp, s, m_manipulator, tStrategyId::empty    ); }; break;
		case  tBrushMode::randomStrat: m_func = [this](GridPoint const & gp, short const s) { m_pGrid->EditSetStrategy (gp, s, m_manipulator, tStrategyId::random   ); }; break;
		case  tBrushMode::cooperate:   m_func = [this](GridPoint const & gp, short const s) { m_pGrid->EditSetStrategy (gp, s, m_manipulator, tStrategyId::cooperate); }; break;
		case  tBrushMode::defect:      m_func = [this](GridPoint const & gp, short const s) { m_pGrid->EditSetStrategy (gp, s, m_manipulator, tStrategyId::defect   ); }; break;
		case  tBrushMode::tit4tat:     m_func = [this](GridPoint const & gp, short const s) { m_pGrid->EditSetStrategy (gp, s, m_manipulator, tStrategyId::tit4tat  ); }; break;
		case  tBrushMode::noAnimals:   m_func = [this](GridPoint const & gp, short const s) { m_pGrid->EditSetStrategy (gp, s, m_manipulator, tStrategyId::empty    ); }; break;
		case  tBrushMode::mutRate:     m_func = [this](GridPoint const & gp, short const s) { m_pGrid->Apply2MutRate   (gp, s, m_manipulator); }; break;
		case  tBrushMode::fertility:   m_func = [this](GridPoint const & gp, short const s) { m_pGrid->Apply2Fertility (gp, s, m_manipulator); }; break;
		case  tBrushMode::food:        m_func = [this](GridPoint const & gp, short const s) { m_pGrid->Apply2FoodStock (gp, s, m_manipulator); }; break;
		case  tBrushMode::fertilizer:  m_func = [this](GridPoint const & gp, short const s) { m_pGrid->Apply2Fertilizer(gp, s, m_manipulator); }; break;
		};
	}

	void SetOperator( tOperator const op ) 
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
  
	void SetShape( tShape const shape )
	{
		m_shape = shape;
		switch ( shape )
		{
		case tShape::Circle:
			m_filter = [&](GridPoint const & gp)
			{ 
				long const lRadius    = static_cast<long>(m_radius);
				long const lRadSquare = lRadius * lRadius;
				long const lx          = static_cast<long>(gp.x);
				long const ly          = static_cast<long>(gp.y);
				long const lDistSquare = lx * lx + ly * ly;
				if ( lDistSquare <= lRadSquare )
				{ 
					short const sReduce = CastToShort(( m_sIntensity * lDistSquare) / lRadSquare);  
					m_func( m_gpCenter + gp, m_sIntensity - sReduce );
				}
			};
			break;

		case tShape::Rect:
			m_filter = [&](GridPoint const & gp)
			{ 
				m_func( m_gpCenter + gp, m_sIntensity );
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
 
    GridPoint  const GetCenter   ( ) const { return m_gpCenter;   }
    GRID_COORD const GetRadius   ( ) const { return m_radius;     }
	short      const GetIntensity( ) const { return m_sIntensity; }
	tShape     const GetShape    ( ) const { return m_shape;      }
    tBrushMode const GetBrushMode( ) const { return m_brushMode;  }
    tOperator  const GetOperator ( ) const { return m_manipulator->GetOperator(); }

protected:
	static bool m_bClassInitialized;

	static Set_Manipulator <short> * m_setMan; 
	static Add_Manipulator <short> * m_addMan; 
	static Subt_Manipulator<short> * m_subMan;    
	static Max_Manipulator <short> * m_maxMan; 
	static Min_Manipulator <short> * m_minMan; 
	static Mean_Manipulator<short> * m_meanMan;
	
	Grid               * m_pGrid;
	Manipulator<short> * m_manipulator;
	GridPointFuncShort   m_func;
	GridPointFunc        m_filter;
    tShape			     m_shape;
    tBrushMode		     m_brushMode;
	short                m_sIntensity;
    GridPoint		     m_gpCenter;
    GRID_COORD	         m_radius;
};
