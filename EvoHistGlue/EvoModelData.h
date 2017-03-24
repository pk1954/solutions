// EvoModelData.h
//

#pragma once

#include "gridPoint.h"
#include "ModelData.h"
#include "EvolutionCore.h"
#include "EvoGenerationCmd.h"

class EvolutionModelData;

class EvoModelData: public ModelData
{
public:
	
    EvoModelData( EvolutionCore * const pCore, EvolutionModelData * pModelDataWork ):
		m_pEvolutionCore( pCore ),
        m_pEvolutionModelData( pModelDataWork )
    { }

    EvoModelData & operator= ( EvoModelData const & );  // noncopyable class 

    ~EvoModelData( );

	virtual void OnNextGeneration()	{ m_pEvolutionCore->Compute   ( m_pEvolutionModelData ); }
	virtual void OnReset()	        { m_pEvolutionCore->ResetModel( m_pEvolutionModelData ); }
    virtual void OnAppCommand( unsigned short, short );
	virtual void CopyModelData( ModelData const * const );

    GridPoint FindGridPoint( IndId const & ) const;

private:
	EvolutionCore      * m_pEvolutionCore;
	EvolutionModelData * m_pEvolutionModelData;
    GridPoint            m_gpEdit;
};
