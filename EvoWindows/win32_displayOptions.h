// win32_displayOptions.h 
//

#pragma once

#include "win32_baseDialog.h"
#include <functional>
#include "gridPoint.h"

class EvolutionCore;

class DspOptWindow : public BaseDialog
{
public:
    DspOptWindow( );
    ~DspOptWindow( );

    void Start( HWND const, EvolutionCore const * const );

	int GetIntValue( GridPoint const gp ) 
	{ 
		return (m_IntValueLambda == nullptr) ? 0 : m_IntValueLambda(gp); 
	}

	void UpdateDspOptionsControls( tBrushMode const );
	void SetDisplayMode( WORD );  // one of IDM_MUT_RATE/IDM_FERTILITY/IDM_FOOD_STOCK/IDM_FERTILIZER
    void SetIndividualsVisible( );
    BOOL AreIndividualsVisible( ) const;

private:
    DspOptWindow             ( DspOptWindow const & );  // noncopyable class 
    DspOptWindow & operator= ( DspOptWindow const & );  // noncopyable class 

	ENERGY_UNITS getNeighborHoodMeanValue( GridPoint const ) const;

	virtual INT_PTR UserProc( UINT const, WPARAM const, LPARAM const );

    EvolutionCore const * m_pCore; 

	std::function<int( GridPoint const )> m_IntValueLambda;
};
