// GridBrush.ixx
//
// EvolutionCore

export module EvolutionCore:GridBrush;

import std;
import Types;
import :EvolutionTypes;
import :GridPoint;
import :GridModel;

export class GridBrush
{
public:
	GridBrush(GridModel * const);

	virtual ~GridBrush() {};

	void Reset();

	void operator()(GridPoint);
    bool operator!=(GridBrush const &) const;

	void SetRadius     (GridCoord   const);
	void SetIntensity  (PERCENT      const); 
	void SetBrushMode  (tBrushMode   const);
	void SetManipulator(tManipulator const);
	void SetShape      (tShape       const);

    GridCoord   const GetRadius     () const { return m_radius;      }
	PERCENT      const GetIntensity  () const { return m_intensity;   }
	tShape       const GetShape      () const { return m_shape;       }
    tBrushMode   const GetBrushMode  () const { return m_brushMode;   }
    tManipulator const GetManipulator() const { return m_manipulator; }

private:
	
	GridModel          * m_pGrid;
	tShape			m_shape;
    tBrushMode		m_brushMode;
	tManipulator	m_manipulator;
	PERCENT         m_intensity;
    GridCoord	    m_radius;
	ManipulatorFunc m_manFunc;

	function<void   (GridPoint const, short const)> m_func;
    function<PERCENT(GridPoint const)>              m_filter;
};
