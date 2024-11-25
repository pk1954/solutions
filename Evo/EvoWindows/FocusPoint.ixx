// FocusPoint.ixx
//
// EvoWindows

export module FocusPoint;

import EvoCoreLib;
import Observable;
import HistoryLib;
import RootWindow;

export class FocusPoint: public Observable
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

	bool const IsInGrid() const 
	{ 
		return ::IsInGrid(m_gp); 
	}

	bool const IsAlive(EvolutionCore const & core) const 
	{ 
		return core.IsAlive(m_gp); 
	}

	bool const IsDead(EvolutionCore const & core) const 
	{ 
		return core.IsDead(m_gp); 
	}

	bool const IsDefined(EvolutionCore const & core) const 
	{ 
		return core.IsDefined(m_gp); 
	}

private:
    GridPoint m_gp;
};
