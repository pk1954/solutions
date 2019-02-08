// win32_focusPoint.h
//

#pragma once

#include "HistoryGeneration.h"
#include "win32_viewCollection.h"

class RootWindow;
class EvoHistorySysGlue;
class EvolutionCore;

class FocusPoint
{
public:
    FocusPoint( );
	virtual ~FocusPoint( ) {};

    void Start( EvoHistorySysGlue *, EvolutionCore * );

    void SetFocusPoint( GridPoint const );

	void AttachFocusPointObserver( RootWindow * pRootWin )
	{
		m_ViewCollection.AttachObserver( pRootWin );
	}
    
	GridPoint const GetGridPoint( ) const 
	{ 
		return m_gp; 
	}

	HIST_GENERATION const GetGenBirth( ) const 
	{ 
		return m_genBirth; 
	}

	HIST_GENERATION const GetGenDeath( ) const 
	{ 
		return m_genDeath; 
	}

	BOOL const IsInGrid( ) const 
	{ 
		return ::IsInGrid( m_gp ); 
	}

	BOOL const IsAlive( ) const 
	{ 
		return m_pCore->IsAlive( m_gp ); 
	}

	BOOL const IsDead( ) const 
	{ 
		return m_pCore->IsDead( m_gp ); 
	}

	BOOL const IsDefined( ) const 
	{ 
		return m_pCore->IsDefined( m_gp ); 
	}

private:
    EvoHistorySysGlue * m_pEvoHistGlue;
    EvolutionCore     * m_pCore;
    ViewCollection      m_ViewCollection;
    HIST_GENERATION     m_genBirth;
    HIST_GENERATION     m_genDeath;
    GridPoint           m_gp;
};
