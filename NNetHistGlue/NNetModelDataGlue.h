// NNetModelDataGlue.h
//
// Glue code for adapting EVOLUTION to HISTORY system
//
// NNetModelDataGlue is ** below ** HISTORY (is called by HISTORY)
//
// NNetHistGlue

#pragma once

#include "ModelData.h"
#include "NNetModel.h"
#include "win32_stopwatch.h"

class NNetModelDataGlue: public ModelData
{
public:
	
    NNetModelDataGlue( )
    { 
		m_pNNetModel = NNetModel::CreateModel( );
	}

	~NNetModelDataGlue( )
	{
		NNetModel::DestroyCore( m_pNNetModel );
	}

    NNetModelDataGlue & operator= ( NNetModelDataGlue const & );  // noncopyable class 

	virtual void Compute( )
	{
		m_pNNetModel->Compute( );
	}

	virtual void ResetAll( )
	{
		m_pNNetModel->ResetAll( );
	}

	NNetModel * GetNNetModel( )
	{
		return m_pNNetModel;
	}

	NNetModel const * GetNNetModelC( ) const
	{
		return m_pNNetModel;
	}

	virtual BYTES GetModelSize( ) const
	{
		return m_pNNetModel->GetCoreSize( ) + BYTES(sizeof(NNetModelDataGlue));
	}

	virtual void CopyFrom( ModelData const * const src )
	{
//		stopwatch.Start();
		m_pNNetModel->CopyModelData( static_cast< NNetModelDataGlue const * const >( src )->m_pNNetModel );
//		stopwatch.Stop( L"Copy model" );
	}

	virtual void OnAppCommand( GenerationCmd const );

private:
   static Stopwatch stopwatch;

   NNetModel * m_pNNetModel;
};
