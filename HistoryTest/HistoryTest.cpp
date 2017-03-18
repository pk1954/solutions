/// HistoryTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "script.h"
#include "BasicHistCacheItem.h"
#include "NextGenFunctor.h"
#include "HistorySystem.h"

using namespace std;

class ModelData
{
public:
	ModelData( int iCounter ): 
		m_iDataTest( iCounter ),
		m_iDataApp( 0 )
	{ }
	
	void Reset( )   { m_iDataApp = 0; }
	void Compute( ) { -- m_iDataApp; }

	virtual void CopyModelData( ModelData const * const src )
    {
        * this = * src;
    }

private:
	int m_iDataTest;
	int m_iDataApp;
};

class TestHistCacheItem : public BasicHistCacheItem
{
public:

	TestHistCacheItem( ) : 
		m_pModelData( new ModelData( m_iCounter ))
	{ }

	TestHistCacheItem( ModelData * pModel ) : 
		m_pModelData( pModel )
	{ }

    ~TestHistCacheItem( )
    {
//        if ( m_pModelData != nullptr )
//            delete m_pModelData;
    }

	void Reset( )   { GetModelData( )->Reset( ); }
	void Compute( ) { GetModelData( )->Compute( ); }

    virtual BasicHistCacheItem * CreateItem( )
	{
		return new TestHistCacheItem( );
	}

    virtual void CopyModelData( BasicHistCacheItem const * const pSrc )
	{
        GetModelData( )->CopyModelData( static_cast<TestHistCacheItem const * const>( pSrc )->GetModelDataC( ) );
	}

private:
    ModelData * const m_pModelData;

	static int m_iCounter;
};

int TestHistCacheItem::m_iCounter = 0;

class TestGenFunctor : public NextGenFunctor
{
public:
    TestGenFunctor( ) :
		m_pModel( nullptr )
	{ }

    TestGenFunctor( ModelData * pModel ) :
		m_pModel( pModel )
	{ }

	virtual void OnNextGeneration() const
	{
		m_pModel->Compute( );
	}

	virtual void OnReset() const
	{
    	m_pModel->Reset( );
	}

    virtual void OnAppCommand ( unsigned short usCmd, short sParam ) const
    {
		assert( false );
	}

private:

	ModelData * m_pModel;
};

int _tmain( int argc, _TCHAR* argv[] )
{
	HistorySystem     historySys;
	ModelData         modelData( 0 );
	TestHistCacheItem itemWork( & modelData );
	TestGenFunctor    genFunctor( & modelData );

	historySys.InitHistorySystem
	( 
		20,    // # of cache slots
		1000,  // # of generations
		& genFunctor,
		& itemWork
	);

	return 0;
}
