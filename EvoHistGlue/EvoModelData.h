// EvoModelData.h
//
//
// Glue code for adapting EVOLUTION to HISTORY system
//
// EvoHistorySys is ** below ** HISTORY (is called by HISTORY)
//

#pragma once

#include "boolOp.h"
#include "gridPoint.h"
#include "ModelData.h"

class EvolutionModelData;
class EvolutionCore;

class EvoModelData: public ModelData
{
public:
	
    EvoModelData
	( 
		EvolutionModelData * const pModelDataWork, 
        EvolutionCore      * const pCore
	):
        m_pEvolutionModelData( pModelDataWork ),
		m_pEvolutionCore( pCore )
    { }

    EvoModelData & operator= ( EvoModelData const & );  // noncopyable class 

    ~EvoModelData( );

    virtual void OnAppCommand( unsigned short const, unsigned short const );
	virtual void CopyModelData( ModelData const * const );

    GridPoint FindGridPoint( IndId const & ) const;

private:
	EvolutionModelData * m_pEvolutionModelData;
    EvolutionCore      * m_pEvolutionCore;
};
