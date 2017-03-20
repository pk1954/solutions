/// HistoryTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "script.h"
#include "ModelData.h"
#include "HistorySystem.h"

using namespace std;

class HistTestModelData: public ModelData
{
public:
	HistTestModelData( int iCounter ): 
		m_iDataTest( iCounter ),
		m_iDataApp( 0 )
	{ }
	
	virtual void CopyModelData( ModelData const * const src )
    {
        * this = * static_cast< HistTestModelData const * const >( src );
    }

	virtual void OnNextGeneration()
	{
		++ m_iDataApp;;
	}

	virtual void OnReset( )
	{
    	 m_iDataApp = 0;;
	}

    virtual void OnAppCommand ( unsigned short usCmd, short sParam )
    {
		assert( false );
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

int _tmain( int argc, _TCHAR* argv[] )
{
	static const int NR_OF_SLOTS = 2;

	HistorySystem      * pHistorySys = HistorySystem::CreateHistorySystem( );
	HistTestModelFactory modelFactory;
	HistTestModelData    modelData( 0 );

	pHistorySys->InitHistorySystem
	( 
		NR_OF_SLOTS,    // # of cache slots
		1000,           // # of generations
		& modelData,
		& modelFactory
	);

	for ( int i = 1; i < NR_OF_SLOTS; ++i )
		pHistorySys->AddHistorySlot( );

	return 0;
}
