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
#include "NNetModelImporter.h"
#include "NNetModelExporter.h"
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

class ConsImportTermination : public ImportTermination
{
public:
	virtual void Reaction( Result const res, wstring const name )
	{
		switch ( res )
		{
		case ImportTermination::Result::ok:
			wcout << L"model file ok";
			break;

		case ImportTermination::Result::fileNotFound:
			wcout << L"+++ could not find model file " << name.c_str() << endl;
			break;

		case ImportTermination::Result::errorInFile:
			wcout << L"+++ error in model file";
			break;

		default:
			assert( false );
		}
		wcout << endl;
	};
};

int main( int argc, char * argv [ ], char * envp [ ] )
{
	wcout << VER_PRODUCTNAME_STR << L" " << VER_FILE_DESCRIPTION_STR << endl;
	wcout << L"Build at " << __DATE__ << L" " << __TIME__ << endl;

	NNetModelReaderInterface m_nmri                   { };
	NNetModelWriterInterface m_nmwi                   { };
	NNetModelCommands        m_modelCommands          { };
	NNetModelImporter        m_modelImporter          { };
	NNetModelExporter        m_modelExporter          { };
	NNetModel                m_model                  { };
	Observable               m_staticModelObservable  { };
	Observable               m_dynamicModelObservable { };
	Script                   m_script                 { };
	CommandStack             m_cmdStack               { };
	MonitorData              m_monitorData            { };
	Animation                m_animationDummy         { };

	DefineUtilityWrapperFunctions( );
	DefineNNetWrappers( & m_modelCommands );

	SignalFactory:: Initialize( m_nmri, m_dynamicModelObservable, m_animationDummy );
	Shape::Initialize( m_model.GetParams() );

	m_modelCommands.Initialize( & m_nmri, & m_nmwi, & m_modelImporter, & m_dynamicModelObservable, & m_cmdStack	);
	m_modelImporter.Initialize(	nullptr );
	m_modelExporter.Initialize( & m_nmri );

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

	m_modelImporter.Import( L"std.mod", make_unique<ConsImportTermination>() );

	if ( ProcessNNetScript( m_script, m_nmwi.GetShapes(), wstrInputFile ) )
		wcout << L" *** NNetSimuConsole terminated successfully";
	else 
		wcout << L"+++ NNetSimuConsole terminated with error";
	wcout << endl;
}
