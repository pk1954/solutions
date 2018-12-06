/// HistoryTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "Int24.h"
#include "symtab.h"
#include "errhndl.h"
#include "ModelData.h"
#include "HistoryIterator.h"
#include "HistorySystem.h"
#include "Version.h"

using namespace std;

class HistTestModelData: public ModelData
{
public:
	HistTestModelData( int iCounter ): 
		m_iDataTest( iCounter ),
		m_iDataApp( 0 )
	{ }

	virtual int GetModelSize()
	{
		return sizeof( HistTestModelData );
	}

	virtual void CopyFrom( ModelData const * const src )
    {
        * this = * static_cast< HistTestModelData const * const >( src );
    }

    virtual void OnAppCommand( GenerationCmd const cmd )
    {
//		wcout << endl << L"OnAppCommand( " << cmd.GetCommand() << L", " << cmd.GetParam() << L" )" << endl;
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

    for ( auto iRun = iter->Set2Oldest( ); iRun != -1; iRun = iter->Set2Junior( ) )
        wcout << iter->GetCurrentGeneration( ) << L" ";

	wcout << endl;

	delete iter;
}

void gotoGeneration( HistorySystem * const pHistorySys, HIST_GENERATION const histGenDemanded )
{
	while( histGenDemanded != pHistorySys->GetCurrentGeneration( ) )
	{
		if ( histGenDemanded > pHistorySys->GetYoungestGeneration( ) )
			pHistorySys->CreateAppCommand( GenerationCmd::ApplicationCmd(static_cast<tGenCmd>( 42 ), 0) );
		else
		{
	    	if ( ! pHistorySys->ApproachHistGen( histGenDemanded ) )
		    	wcout << L"Error: Generation " << histGenDemanded << L" cannot be reached";
		}
    	wcout << L"Generation: " << histGenDemanded << L" - " << pHistorySys->GetCurrentGeneration( ) << L" Slots: ";
		showHistorySlots( pHistorySys );
	}
}

void DoTest( )
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
		& modelFactory,
		nullptr,
		GenerationCmd::ApplicationCmd( tGenCmd::FIRST_APP_CMD, 0 )
	);

	wcout << L"*** Create " << NR_OF_SLOTS << L" history slots" << endl;

	for ( int i = 1; i < NR_OF_SLOTS; ++i )
		pHistorySys->AddHistorySlot( );

	assert( pHistorySys->GetNrOfHistCacheSlots( ) == NR_OF_SLOTS );

	wcout << endl << L"*** Iterate thru generations" << endl << endl;

	for ( histGenDemanded = 1; histGenDemanded < 30; ++histGenDemanded )
		gotoGeneration( pHistorySys, histGenDemanded );

	wcout << endl << L"*** Now backwards" << endl << endl;

	for ( histGenDemanded = 28; histGenDemanded >= 0; --histGenDemanded )
		gotoGeneration( pHistorySys, histGenDemanded );

	wcout << endl << L"*** HistoryTest finished" << endl;
}

class WrapHistoryTest_1 : public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
		try
		{
			DoTest( );
		}
		catch (...)
		{
			wcout << endl << L"+++ Error in HistoryTest" << endl;
		}
    }
};

int _tmain( int argc, char * argv[] )
{
	wcout << VER_PRODUCTNAME_STR << L" " << VER_FILE_DESCRIPTION_STR << endl;
	wcout << L"Build at " << __DATE__ << L" " << __TIME__ << endl;

	ScriptErrorHandler::ScrSetOutputStream( & wcout );
	DEF_FUNC( HistoryTest_1 );

    wstring wstrInputFile = L"Test_1.in";

	for ( int iCount = 1; iCount < argc; iCount++ )
    {
        std::string strCmd( argv[ iCount ] );

		if ( (strCmd.find( ".in" ) != string::npos) || (strCmd.find( ".IN" ) != string::npos) ) 
		{
			wstrInputFile.assign( strCmd.begin(), strCmd.end() ); 
		}
    }

	Script::ProcessScript( wstrInputFile );

	return 0;
}
