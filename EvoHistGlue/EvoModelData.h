// EvoModelData.h
//

#pragma once

#include <functional>
#include "gridPoint.h"
#include "ModelData.h"
#include "win32_worker_thread.h"

class EvolutionModelData;

class EvoModelData: public ModelData
{
public:
	
    EvoModelData( EvolutionModelData * pModelDataWork, WorkThread * pWorkThread ):
        m_pEvolutionModelData( pModelDataWork ),
		m_pWorkThread( pWorkThread )
    { }

    EvoModelData & operator= ( EvoModelData const & );  // noncopyable class 

    ~EvoModelData( );

    virtual void OnAppCommand( unsigned short const, unsigned short const );
	virtual void CopyModelData( ModelData const * const );

	GridPoint FindGridPoint( const std::function<bool( GridPoint const &)>& ) const;
    GridPoint FindGridPoint( IndId const & ) const;

private:
	EvolutionModelData * m_pEvolutionModelData;
	WorkThread         * m_pWorkThread;
};
