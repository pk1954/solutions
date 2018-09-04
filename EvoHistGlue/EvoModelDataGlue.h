// EvoModelDataGlue.h
//
// Glue code for adapting EVOLUTION to HISTORY system
//
// EvoModelDataGlue is ** below ** HISTORY (is called by HISTORY)
//

#pragma once

#include "BoolOp.h"
#include "Int24.h"
#include "gridPoint.h"
#include "ModelData.h"

class EvolutionModelData;
class EvolutionCore;

class EvoModelDataGlue: public ModelData
{
public:
	
    EvoModelDataGlue
	( 
		EvolutionModelData * const pModelDataWork, 
        EvolutionCore      * const pCore
	):
        m_pEvolutionModelData( pModelDataWork ),
		m_pEvolutionCore( pCore )
    { }

    EvoModelDataGlue & operator= ( EvoModelDataGlue const & );  // noncopyable class 

    ~EvoModelDataGlue( );

    virtual void OnAppCommand( GenerationCmd const );
	virtual void CopyModelData( ModelData const * const );

    GridPoint FindGridPoint( IndId const & ) const;

private:
	EvolutionModelData * m_pEvolutionModelData;
    EvolutionCore      * m_pEvolutionCore;
};
