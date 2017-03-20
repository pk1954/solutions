// EvoModelData.h
//

#pragma once

#include "gridPoint.h"
#include "ModelData.h"
#include "EvolutionCore.h"
#include "EvolutionModelData.h"
#include "EvoGenerationCmd.h"
#include "win32_worker_thread.h"

class EvoModelData: public ModelData
{
public:

    EvoModelData( EvolutionCore * const pCore ):
		m_pEvolutionCore( pCore ),
		m_pWorkThread( nullptr ),
        m_pEvolutionModelData( EvolutionModelData::CreateModelData( ) )
    { }
	
    EvoModelData( EvolutionCore * const pCore, EvolutionModelData * pModelDataWork ):
		m_pEvolutionCore( pCore ),
		m_pWorkThread( nullptr ),
        m_pEvolutionModelData( pModelDataWork )
    { }

    EvoModelData & operator= ( EvoModelData const & );  // noncopyable class 

    ~EvoModelData( )
    {
        delete m_pEvolutionModelData;
    }

	virtual void CopyModelData( ModelData const * const src )
    {
		EvoModelData const * const evoSrc = static_cast< EvoModelData const * const >( src );
        m_pEvolutionModelData->CopyEvolutionModelData( evoSrc->m_pEvolutionModelData );
    }

	virtual void OnNextGeneration()	{ m_pWorkThread->GenerationStep( ); }
	virtual void OnReset()	        { m_pWorkThread->ResetModel( );	}

    virtual void OnAppCommand ( unsigned short usCmd, short sParam  )
    {
        m_pWorkThread->WorkThread::ApplyEditorCommand( static_cast<tEvoCmd>( usCmd ), sParam );
    }

    EvolutionModelData       * GetModelData( )        { return m_pEvolutionModelData; }
    EvolutionModelData const * GetModelDataC( ) const { return m_pEvolutionModelData; }

    GridPoint FindGridPoint( IndId const & id ) const
    { 
        return GetModelDataC( )->FindGridPoint( id );
    }

	void SetWorkThread( WorkThread * pWorkThread )
	{
		m_pWorkThread = pWorkThread;
	}

private:
	EvolutionCore      * m_pEvolutionCore;
	EvolutionModelData * m_pEvolutionModelData;
    WorkThread         * m_pWorkThread;
};

class EvoModelFactory: public ModelFactory
{
public:
	EvoModelFactory( EvolutionCore * pEvolutionCore )
		: m_pEvolutionCore( pEvolutionCore ),
     	  m_pWorkThread   ( nullptr)
	{ }

	void SetWorkThread( WorkThread * pWorkThread )
	{
		m_pWorkThread = pWorkThread;
	}

	virtual ModelData * CreateModelData() const 
	{
		EvoModelData * pEvoModelData = new EvoModelData( m_pEvolutionCore ); 
		pEvoModelData->SetWorkThread( m_pWorkThread );
		return pEvoModelData;
	}

private:
	EvolutionCore * m_pEvolutionCore;
    WorkThread    * m_pWorkThread;
};
