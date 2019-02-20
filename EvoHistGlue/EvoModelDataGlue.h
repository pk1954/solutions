// EvoModelDataGlue.h
//
// Glue code for adapting EVOLUTION to HISTORY system
//
// EvoModelDataGlue is ** below ** HISTORY (is called by HISTORY)
//

#pragma once

#include "BoolOp.h"
#include "gridPoint.h"
#include "gridRect.h"
#include "GridDimensions.h"
#include "ModelData.h"
#include "EvolutionCore.h"

class EvoModelDataGlue: public ModelData
{
public:
	
    EvoModelDataGlue( EvolutionCore * pCore )
    { 
		m_pEvolutionCore = pCore;
	}

    EvoModelDataGlue & operator= ( EvoModelDataGlue const & );  // noncopyable class 

    ~EvoModelDataGlue( );

	virtual int GetModelSize()
	{
		return EvolutionCore::GetModelSize( ) + sizeof(EvoModelDataGlue);
	}

	virtual void OnAppCommand( GenerationCmd const );
	virtual void CopyFrom( ModelData const * const );

    GridPoint FindGridPoint( IND_ID const &, GridRect const & = GRID_RECT_FULL() ) const;

private:
    EvolutionCore * m_pEvolutionCore;
};
