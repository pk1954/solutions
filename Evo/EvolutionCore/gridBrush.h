// gridBrush.h
//
// EvolutionCore

module;

import EvolutionTypes;

import GridPoint;

class Grid;

class GridBrush
{
public:
	GridBrush(Grid * const);

	virtual ~GridBrush() {};

	void Reset();

	void operator()(GridPoint);
    bool operator!=(GridBrush const &) const;

	void SetRadius     (GRID_COORD   const);
	void SetIntensity  (PERCENT      const); 
	void SetBrushMode  (tBrushMode   const);
	void SetManipulator(tManipulator const);
	void SetShape      (tShape       const);

    GRID_COORD   const GetRadius     () const { return m_radius;      }
	PERCENT      const GetIntensity  () const { return m_intensity;   }
	tShape       const GetShape      () const { return m_shape;       }
    tBrushMode   const GetBrushMode  () const { return m_brushMode;   }
    tManipulator const GetManipulator() const { return m_manipulator; }

private:
	
	Grid          * m_pGrid;
	tShape			m_shape;
    tBrushMode		m_brushMode;
	tManipulator	m_manipulator;
	PERCENT         m_intensity;
    GRID_COORD	    m_radius;
	ManipulatorFunc m_manFunc;

	function<void   (GridPoint const, short const)> m_func;
    function<PERCENT(GridPoint const)>              m_filter;
};
