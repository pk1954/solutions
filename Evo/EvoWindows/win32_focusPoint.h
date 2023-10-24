// win32_focusPoint.h
//
// EvoWindows

module;

import GridDimensions;
#include "GridPoint.h"
#include "EvolutionCore.h"
#include "observable.h"
#include "HistoryGeneration.h"

class RootWindow;
class EvolutionCore;

class FocusPoint: public Observable
{
public:
	FocusPoint() :
		m_gp(GP_NULL)
	{ };
	virtual ~FocusPoint() {};

	void SetFocusPoint(GridPoint const gpNew)
	{
		if (gpNew != m_gp)
		{
			m_gp = gpNew;
			NotifyAll(false);
		}
	}

	GridPoint const GetGridPoint() const 
	{ 
		return m_gp; 
	}

	BOOL const IsInGrid() const 
	{ 
		return ::IsInGrid(m_gp); 
	}

	BOOL const IsAlive(EvolutionCore const & core) const 
	{ 
		return core.IsAlive(m_gp); 
	}

	BOOL const IsDead(EvolutionCore const & core) const 
	{ 
		return core.IsDead(m_gp); 
	}

	BOOL const IsDefined(EvolutionCore const & core) const 
	{ 
		return core.IsDefined(m_gp); 
	}

private:
    GridPoint m_gp;
};
