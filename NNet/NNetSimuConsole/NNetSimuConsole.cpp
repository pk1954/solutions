// NNetSimuConsole.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "stdafx.h"
#include "version.h"
#include <iostream>
#include "script.h"
#include "trace.h"
#include "UtilityWrappers.h"
#include "CommandStack.h"
#include "MonitorData.h"
#include "SignalFactory.h"
#include "ModelDescription.h"
#include "NNetModelCommands.h"
#include "NNetModelReaderInterface.h"
#include "NNetModelWriterInterface.h"
#include "NNetModelImport.h"
#include "NNetModelExport.h"
#include "NNetWrappers.h"
#include "NNetError.h"

using std::wcout;
using std::endl;
using std::string;
using std::wofstream;

class Animation : public AnimationInterface
{
public:
	virtual void Initialize( Observable * const p ) {}
	virtual void Start( ShapeId const id ) 	{}
	virtual void Start( MicroMeterCircle const * const pCircle ) {};
	virtual void Stop( ) {}
};

class ConsReadModelResult : public ReadModelResult
{
public:
	ConsReadModelResult( ) { }

	virtual void Reaction( tResult const res, wstring const name )
	{
		switch ( res )
		{
		case ReadModelResult::tResult::ok:
			wcout << L"model file ok";
			break;

		case ReadModelResult::tResult::fileNotFound:
			wcout << L"+++ could not find model file " << name.c_str() << endl;
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
};

int main( int argc, char * argv [ ], char * envp [ ] )
{
	wcout << VER_PRODUCTNAME_STR << L" " << VER_FILE_DESCRIPTION_STR << endl;
	wcout << L"Build at " << __DATE__ << L" " << __TIME__ << endl;

	NNetModelReaderInterface m_nmri                     { };
	NNetModelWriterInterface m_nmwi                     { };
	NNetModelCommands        m_modelCommands            { };
	NNetModelImport          m_modelImport              { };
	NNetModelExport          m_modelExport              { };
	NNetModel                m_model                    { };
	Param                    m_parameters               { };
	Observable               m_modelTimeObservable      { };
	Observable               m_staticModelObservable    { };
	Observable               m_dynamicModelObservable   { };
	Observable               m_unsavedChangesObservable { };
	Script                   m_script                   { };
	CommandStack             m_cmdStack                 { };
	MonitorData              m_monitorData              { };
	Animation                m_animationDummy           { };
	ReadModelResult        * m_pReadModelResult         { nullptr };

	DefineUtilityWrapperFunctions( );
	DefineNNetWrappers( & m_modelCommands );

	m_pReadModelResult = new ConsReadModelResult( );
	SignalFactory::Initialize
	( 
		m_nmri,
		m_parameters, 
		m_dynamicModelObservable,
		m_animationDummy
		
	);
	m_modelCommands.Initialize
	( 
		& m_nmwi, 
		& m_parameters, 
		& m_cmdStack, 
		& m_dynamicModelObservable
	);
	m_model.Initialize
	( 
		& m_parameters, 
		& m_staticModelObservable, 
		& m_dynamicModelObservable, 
		& m_modelTimeObservable,
		& m_unsavedChangesObservable
	);
	m_modelImport.Initialize
	(
		& m_nmwi,
		nullptr,
		m_pReadModelResult
	);
	m_modelExport.Initialize( & m_nmri );
	m_nmwi.Start( & m_model );

	wstring wstrInputFile{} ; // = L"D:\\SW-projects\\Solutions\\NNet\\Tests\\test_1.in";

	for ( int iCount = 1; iCount < argc; iCount++ )
	{
		string strCmd( argv[ iCount ] );

		if ( (strCmd.find( ".in" ) != string::npos) || (strCmd.find( ".IN" ) != string::npos) ) 
		{
			wstrInputFile.assign( strCmd.begin(), strCmd.end() ); 
		}
	}

	m_modelImport.Import( L"std.mod", nullptr, false );

	if ( ProcessNNetScript( & m_script, m_nmwi.GetShapes(), wstrInputFile ) )
		wcout << L" *** NNetSimuConsole terminated successfully";
	else 
		wcout << L"+++ NNetSimuConsole terminated with error";
	wcout << endl;
}
