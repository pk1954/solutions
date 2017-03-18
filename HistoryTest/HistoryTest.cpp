/// HistoryTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "script.h"
#include "BasicHistCacheItem.h"
#include "generationCmd.h"
#include "NextGenFunctor.h"
#include "HistorySystem.h"

using namespace std;

class TestHistCacheItem : public BasicHistCacheItem
{
public:

	TestHistCacheItem( ) : 
		m_iDataTest( m_iCounter ),
		m_iDataApp( 0 )
	{ }

	void Reset( )   { m_iDataApp = 0; }
	void Compute( ) { -- m_iDataApp; }

    virtual BasicHistCacheItem * CreateItem( )
	{
		return new TestHistCacheItem( );
	}

    virtual void CopyCacheItem( BasicHistCacheItem const * const rhs )
	{
		m_iDataTest = static_cast<TestHistCacheItem const * const>( rhs )->m_iDataTest;
		m_iDataApp  = static_cast<TestHistCacheItem const * const>( rhs )->m_iDataApp;
	}

private:
	int m_iDataTest;
	int m_iDataApp;

	static int m_iCounter;
};

int TestHistCacheItem::m_iCounter = 0;

class TestGenFunctor : public NextGenFunctor
{
public:
    TestGenFunctor( ) :
		m_pItem( nullptr )
	{ }

    TestGenFunctor( TestHistCacheItem * pItem ) :
		m_pItem( pItem )
	{ }

    virtual void operator() ( GenerationCmd genCmd ) const
    {
        tGenCmd const cmd = genCmd.GetCommand( );

        switch ( cmd )
        {
        case tGenCmd::nextGen:
			m_pItem->Compute( );
            break;

        case tGenCmd::reset:
			m_pItem->Reset( );
            break;

        case tGenCmd::undefined:
        case tGenCmd::cached:
            assert( false );
			break;

        default:
			break;
        }
    }

private:

	TestHistCacheItem * m_pItem;
};

int _tmain( int argc, _TCHAR* argv[] )
{
	HistorySystem     historySys;
	TestHistCacheItem itemWork;
	TestGenFunctor    genFunctor( & itemWork );

	historySys.InitHistorySystem
	( 
		20,    // # of cache slots
		1000,  // # of generations
		& genFunctor,
		& itemWork
	);

	return 0;
}
