// NNetSimuConsole.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "stdafx.h"
#include "version.h"
#include <iostream>
#include "SCRIPT.H"
#include "trace.h"
#include "UtilityWrappers.h"
#include "NNetModelWriterInterface.h"
#include "NNetModelStorage.h"
#include "NNetWrappers.h"
#include "NNetError.h"

using std::wcout;
using std::endl;
using std::string;
using std::wofstream;

class ConsReadModelResult : public ReadModelResult
{
public:
	ConsReadModelResult( NNetModelStorage const * const pStorage )
		: m_pStorage( pStorage )
	{ }

	virtual void Reaction( tResult const res )
	{
		switch ( res )
		{
		case ReadModelResult::tResult::ok:
			wcout << L"model file ok";
			break;

		case ReadModelResult::tResult::fileNotFound:
			wcout << L"+++ could not find model file " << m_pStorage->GetModelPath().c_str() << endl;
			break;

		case ReadModelResult::tResult::errorInFile:
			wcout << L"+++ error in model file";
			break;

		default:
			assert( false );
		}
		wcout << endl;
	};

private:
	NNetModelStorage const * const m_pStorage;
};

int main( int argc, char * argv [ ], char * envp [ ] )
{
	wcout << VER_PRODUCTNAME_STR << L" " << VER_FILE_DESCRIPTION_STR << endl;
	wcout << L"Build at " << __DATE__ << L" " << __TIME__ << endl;

	wostream & m_traceStream { wcout };

	NNetModelWriterInterface m_modelWriterInterface     { };
	NNetModelStorage         m_modelStorage             { };
	NNetModel                m_model                    { };
	Param                    m_parameters               { };
	Observable               m_modelTimeObservable      { };
	Observable               m_staticModelObservable    { };
	Observable               m_dynamicModelObservable   { };
	Observable               m_unsavedChangesObservable { };
	Script                   m_script                   { };
	ReadModelResult        * m_pReadModelResult         { nullptr };

	DefineUtilityWrapperFunctions( );
	DefineNNetWrappers( & m_modelWriterInterface, & m_modelStorage );

	m_pReadModelResult = new ConsReadModelResult( & m_modelStorage );
	m_modelWriterInterface.Initialize( & m_traceStream );
	m_model.Initialize
	( 
		& m_parameters, 
		& m_staticModelObservable, 
		& m_dynamicModelObservable, 
		& m_modelTimeObservable 
	);
	m_modelStorage.Initialize
	( 
		& m_model, 
		& m_parameters, 
		& m_unsavedChangesObservable,
		& m_script,
		m_pReadModelResult 
	);
	m_modelWriterInterface.Start( & m_model );

	wstring wstrInputFile = L"D:\\SW-projects\\Solutions\\NNet\\Tests\\test_1.in";

	for ( int iCount = 1; iCount < argc; iCount++ )
	{
		string strCmd( argv[ iCount ] );

		if ( (strCmd.find( ".in" ) != string::npos) || (strCmd.find( ".IN" ) != string::npos) ) 
		{
			wstrInputFile.assign( strCmd.begin(), strCmd.end() ); 
		}
	}

	if ( ProcessNNetScript( & m_script, & m_model, wstrInputFile ) )
		wcout << L" ***** NNetSimuConsole terminated successfully";
	else 
		wcout << L"+++ NNetSimuConsole terminated with error";
	wcout << endl;
}
