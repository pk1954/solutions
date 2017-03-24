/// HistoryTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "script.h"
#include "ModelData.h"
#include "HistoryIterator.h"
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

    virtual void OnAppCommand( unsigned short usCmd, short sParam )
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

void showHistorySlots( HistorySystem * const pHistorySys )
{
	HistoryIterator * iter = pHistorySys->CreateHistoryIterator( );

    for ( int iRun = iter->Set2Oldest( ); iRun != -1; iRun = iter->Set2Junior( ) )
        wcout << iter->GetCurrentGeneration( ) << L" ";

	wcout << endl;

	delete iter;
}

void gotoGeneration( HistorySystem * const pHistorySys, HIST_GENERATION const histGenDemanded )
{
	while( histGenDemanded != pHistorySys->GetCurrentGeneration( ) )
	{
		pHistorySys->ApproachHistGen( histGenDemanded );
    	wcout << L"Generation: " << histGenDemanded << L" - " << pHistorySys->GetCurrentGeneration( ) << L" Slots: ";
		showHistorySlots( pHistorySys );
	}
}

int _tmain( int argc, _TCHAR* argv[] )
{
	static const int NR_OF_SLOTS = 10;

	HistorySystem      * pHistorySys = HistorySystem::CreateHistorySystem( );
	HistTestModelFactory modelFactory;
	HistTestModelData    modelData( 0 );
	HIST_GENERATION      histGenDemanded;

	pHistorySys->InitHistorySystem
	( 
		NR_OF_SLOTS,    // # of cache slots
		1000,           // # of generations
		& modelData,
		& modelFactory
	);

	wcout << L"Create " << NR_OF_SLOTS << L" history slots" << endl << endl;

	for ( int i = 1; i < NR_OF_SLOTS; ++i )
		pHistorySys->AddHistorySlot( );

	assert( pHistorySys->GetNrOfHistCacheSlots( ) == NR_OF_SLOTS );

	for ( histGenDemanded = 1; histGenDemanded < 30; ++histGenDemanded )
		gotoGeneration( pHistorySys, histGenDemanded );

	wcout << endl << L"Now backwards" << endl << endl;

	for ( histGenDemanded = 28; histGenDemanded >= 0; --histGenDemanded )
		gotoGeneration( pHistorySys, histGenDemanded );

	return 0;
}
