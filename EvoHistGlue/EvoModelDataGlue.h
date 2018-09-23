// EvoModelDataGlue.h
//
// Glue code for adapting EVOLUTION to HISTORY system
//
// EvoModelDataGlue is ** below ** HISTORY (is called by HISTORY)
//

#pragma once

#include "BoolOp.h"
#include "gridPoint.h"
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

    virtual void OnAppCommand( GenerationCmd const );
	virtual void CopyFrom( ModelData const * const );

    GridPoint FindGridPoint( IndId const & ) const;

private:
    EvolutionCore * m_pEvolutionCore;
};
