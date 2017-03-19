/// HistoryTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "script.h"
#include "HistCacheItem.h"
#include "NextGenFunctor.h"
#include "HistorySystem.h"

using namespace std;

class HistTestModelData: public ModelData
{
public:
	HistTestModelData( int iCounter ): 
		m_iDataTest( iCounter ),
		m_iDataApp( 0 )
	{ }
	
	virtual void Reset( )   { m_iDataApp = 0; }
	virtual void Compute( ) { -- m_iDataApp; }

	virtual void CopyModelData( ModelData const * const src )
    {
        * this = * static_cast< HistTestModelData const * const >( src );
    }

private:
	int m_iDataTest;
	int m_iDataApp;
};

class HistTestModelFactory: public ModelFactory
{
public:
	virtual ModelData * CreateModelData() const 
	{
		return new HistTestModelData( 0 );  // TODO: is 0 ok???
	}
};

class TestGenFunctor : public NextGenFunctor
{
public:
    TestGenFunctor( ) :
		m_pModel( nullptr )
	{ }

    TestGenFunctor( HistTestModelData * pModel ) :
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

	HistTestModelData * m_pModel;
};

int _tmain( int argc, _TCHAR* argv[] )
{
	static const int NR_OF_SLOTS = 2;

	HistorySystem        historySys;
	HistTestModelFactory modelFactory;
	HistTestModelData    modelData( 0 );
	TestGenFunctor       genFunctor( & modelData );

	historySys.InitHistorySystem
	( 
		NR_OF_SLOTS,    // # of cache slots
		1000,           // # of generations
		& genFunctor,
		& modelFactory
	);

	for ( int i = 0; i < NR_OF_SLOTS; ++i )
		historySys.AddHistorySlot( );

	return 0;
}
