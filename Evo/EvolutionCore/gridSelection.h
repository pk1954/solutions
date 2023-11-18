// gridSelection.h
//
// EvolutionCore

module;

import GridDimensions;

import GridRect;

class GridSelection
{
public:
	static void ResetSelection()
	{ 
		m_gridRectSelection.SetZero(); 
	};

	static void SetSelection(GridRect const & rect) 
	{ 
		m_gridRectSelection = rect; 
	}

	static bool SelectionIsEmpty()  
	{ 
		return m_gridRectSelection.IsEmpty(); 
	}

	static bool SelectionIsNotEmpty()  
	{ 
		return m_gridRectSelection.IsNotEmpty(); 
	}

	static GridRect GetSelection()  
	{ 
		return m_gridRectSelection.IsEmpty() 
		       ? GridDimensions::GridRectFull() 
			   : m_gridRectSelection; 
	}

private:
	static GridRect m_gridRectSelection;
};