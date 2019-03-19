// gridSelection.h
//
// EvolutionCoreInterface

#pragma once

#include "GridDimensions.h"
#include "gridRect.h"

class GridSelection
{
public:
	static void ResetSelection()
	{ 
		m_gridRectSelection.Reset(); 
	};

	static void SetSelection( GridRect const & rect ) 
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
		return m_gridRectSelection.IsEmpty( ) 
		       ? GridDimensions::GridRectFull() 
			   : m_gridRectSelection; 
	}

private:
	static GridRect m_gridRectSelection;
};