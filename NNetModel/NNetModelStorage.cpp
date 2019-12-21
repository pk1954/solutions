// NNetModelStorage.cpp
//
// NNetModel

#include "stdafx.h"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>
#include "assert.h"
#include "SCRIPT.H"
#include "symtab.h"
#include "errhndl.h"
#include "MoreTypes.h"
#include "NNetModel.h"
#include "win32_script.h"
#include "NNetModelStorage.h"

using std::wcout;
using std::endl;
using std::put_time;

static float const PROTOCOL_VERSION { 1.3f };

////////////////////////// Read /////////////////////////////////////////////

class WrapProtocol : public Script_Functor
{
public:
	WrapProtocol( NNetModel * const pNNetModel ) :
		m_pModel( pNNetModel )
	{ };

	virtual void operator() ( Script & script ) const 
	{
		script.ScrReadString( L"version" );
		double dVersion = script.ScrReadFloat();
	}

private:
	NNetModel * m_pModel;
};

class WrapCreateShape : public Script_Functor
{
public:
	WrapCreateShape( NNetModel * const pNNetModel ) :
		m_pModel( pNNetModel )
	{ };

	virtual void operator() ( Script & script ) const
	{   
		ShapeId     const idFromScript{ script.ScrReadLong() };
		tShapeType  const shapeType   { static_cast<tShapeType>(script.ScrReadInt( )) };
		Shape     *       pShape      { nullptr };

		script.ScrReadSpecial( L'(' );
		if ( shapeType == tShapeType::pipeline )
		{
			ShapeId const idStart { script.ScrReadLong() };
			script.ScrReadSpecial( L'-' );
			script.ScrReadSpecial( L'>' );
			ShapeId const idEnd { script.ScrReadLong() };
			Pipeline * pPipeline = new Pipeline( m_pModel );
			m_pModel->ConnectPipeline( pPipeline, idFromScript, idStart, idEnd );
			pShape = pPipeline;
		}
		else 
		{
			MicroMeter      const xCoord { CastToFloat( script.ScrReadFloat() ) };
			script.ScrReadSpecial( L'|' );
			MicroMeter      const yCoord { CastToFloat( script.ScrReadFloat() ) };
			MicroMeterPoint const umPosition( xCoord, yCoord );
			switch ( shapeType )
			{
			case tShapeType::inputNeuron:
				pShape = new InputNeuron( m_pModel, umPosition );
				break;

			case tShapeType::neuron:
				pShape = new Neuron( m_pModel, umPosition );
				break;

			case tShapeType::knot:
				pShape = new Knot( m_pModel, umPosition );
				break;

			default:
				assert( false );
				break;
			}
		}
		script.ScrReadSpecial( L')' );

		m_pModel->SetShape( pShape, idFromScript );
		pShape->SetId( idFromScript );
	}

private:

	NNetModel * m_pModel;
};

class WrapGlobalParameter : public Script_Functor
{
public:
	WrapGlobalParameter( NNetModel * const pNNetModel ) :
		m_pModel( pNNetModel )
	{ };

	virtual void operator() ( Script & script ) const 
	{
		tParameter const param( static_cast< tParameter >( script.ScrReadUint() ) );
		script.ScrReadSpecial( L'=' );
		float const fValue { CastToFloat( script.ScrReadFloat() ) };
		m_pModel->SetParameter( param, fValue );
	}

private:
	NNetModel * m_pModel;
};

class WrapNrOfShapes : public Script_Functor
{
public:
	WrapNrOfShapes( NNetModel * const pNNetModel ) :
		m_pModel( pNNetModel )
	{ };

	virtual void operator() ( Script & script ) const 
	{
		script.ScrReadSpecial( L'=' );
		m_pModel->SetNrOfShapes( script.ScrReadLong() );
	}

private:
	NNetModel * m_pModel;
};

class WrapShapeParameter : public Script_Functor
{
public:
	WrapShapeParameter( NNetModel * const pNNetModel ) :
		m_pModel( pNNetModel )
	{ };

	virtual void operator() ( Script & script ) const 
	{
		script.ScrReadString( L"InputNeuron" );
		ShapeId const id ( script.ScrReadUint() );
		tParameter const param( static_cast< tParameter >( script.ScrReadUint() ) );
		assert( param == tParameter::pulseRate );
		script.ScrReadSpecial( L'=' );
		float const fValue { CastToFloat( script.ScrReadFloat() ) };
		m_pModel->SetPulseRate( id, fValue );
	}

private:
	NNetModel * m_pModel;
};

bool NNetModelStorage::Read( wstring const & wstrPath )
{
	Script scriptModel;
	m_pModel->ResetAll();
	wcout << L"NNet model file " << wstrPath;
	if ( scriptModel.ScrProcess( wstrPath ) )
	{
		wcout << L" sucessfully processed" << endl;
		return true;
	}
	else
	{
		wcout << L" missing or bad" << endl;
		return false;
	}
}

////////////////////////// constructor /////////////////////////////////////////////

NNetModelStorage::NNetModelStorage(  NNetModel * const pModel )
	: m_pModel( pModel ),
	  m_wstrPathOfOpenModel( L"" )
{
#define DEF_NNET_FUNC(name) SymbolTable::ScrDefConst( L#name, new Wrap##name##( m_pModel ) )
	DEF_NNET_FUNC( Protocol );
	DEF_NNET_FUNC( GlobalParameter );
	DEF_NNET_FUNC( ShapeParameter );
	DEF_NNET_FUNC( NrOfShapes );
	DEF_NNET_FUNC( CreateShape );
#undef DEF_NET_FUNC

	Apply2AllShapeTypes
	( 
		[&]( tShapeType const & type ) 
		{
			SymbolTable::ScrDefConst( ::GetName( type ), static_cast<unsigned long>(type) );
		}
	);

	m_pModel->Apply2AllParameters
	( 
		[&]( tParameter const & param ) 
		{
			SymbolTable::ScrDefConst( pModel->GetParameterName( param ), static_cast<unsigned long>( param ) );
		}
	);

#undef DEF_NNET_FUNC
}

////////////////////////// Write /////////////////////////////////////////////

void NNetModelStorage::Write( wostream & out )
{
	static int const BUF_SIZE { 128 };

	struct tm tmDest;
	time_t t = std::time( nullptr );
	localtime_s( &tmDest, &t );

	out << L"# NNetModel" << endl;
	out << L"# Created " << put_time( &tmDest, L"%d-%m-%Y %H-%M-%S" ) << endl;

	WCHAR  infoBuf[BUF_SIZE];
	DWORD  bufCharCount = BUF_SIZE;

	// Get and display the name of the computer.
	GetComputerName( infoBuf, &bufCharCount );
	out << L"# Computer name: " << infoBuf << endl;

	// Get and display the user name.
	GetUserName( infoBuf, &bufCharCount );
	out << L"# User name: " << infoBuf << endl;
	out << endl;

	out << L"Protocol version " << PROTOCOL_VERSION << endl;
	out << endl;

	m_pModel->Apply2GlobalParameters
	( 
		[&]( tParameter const & param ) 
		{
			out << L"GlobalParameter " << m_pModel->GetParameterName( param ) << L" = "
			<< m_pModel->GetParameterValue( param ) 
			<< endl; 
		}
	);

	m_CompactIds.resize( m_pModel->GetSizeOfShapeList() );
	ShapeId idCompact( 0 );
	for ( int i = 0; i < m_CompactIds.size( ); ++i )
	{
		m_CompactIds[ i ] = m_pModel->GetConstShape( ShapeId( i ) )
			              ? idCompact++
			              : NO_SHAPE;
	}

	out << endl;
	out << L"NrOfShapes = " << m_CompactIds.size() << endl;
	out << endl;

	m_pModel->Apply2All<BaseKnot>( [&]( BaseKnot & shape ) { WriteShape( out, shape ); } );
	m_pModel->Apply2All<Pipeline>( [&]( Pipeline & shape ) { WriteShape( out, shape ); } );

	out << endl;

	m_pModel->Apply2All<InputNeuron>
	(
		[&]( InputNeuron & inpNeuron )
		{ 
			out << L"ShapeParameter InputNeuron " << getCompactIdVal( inpNeuron.GetId() ) << L" "
				<< m_pModel->GetParameterName( tParameter::pulseRate ) 
				<< L" = " << m_pModel->GetPulseRate( & inpNeuron )
     			<< endl; 
		}
	);
}

void NNetModelStorage::WritePipeline( wostream & out, Shape const & shape )
{
	Pipeline const & pipe { static_cast<Pipeline const &>( shape ) };
	out << getCompactIdVal( pipe.GetStartKnot() ) << L"->" << getCompactIdVal( pipe.GetEndKnot() ) ;
}

void NNetModelStorage::WriteMicroMeterPoint( wostream & out, MicroMeterPoint const & pnt )
{
	out << pnt.GetXvalue() << L"|" << pnt.GetYvalue();
}

void NNetModelStorage::WriteShape( wostream & out, Shape & shape )
{
	if ( shape.IsDefined() )
	{
		out << L"CreateShape " << getCompactIdVal( shape.GetId() ) << L" " << shape.GetName();
		out << L" (";
		switch ( shape.GetShapeType( ) )
		{
			case tShapeType::inputNeuron:
			case tShapeType::neuron:
			case tShapeType::knot:
				WriteMicroMeterPoint( out, static_cast<BaseKnot &>( shape ).GetPosition() );
				break;

			case tShapeType::pipeline:
				WritePipeline( out, shape );
				break;

			default:
				assert( false );
				break;
		}
		out << L")";
		out << endl;
	}
}

/////////////////////////////////////////////

void NNetModelStorage::OpenModel( )
{
	m_wstrPathOfOpenModel = AskForFileName( GetPathOfExecutable( ), L"*.mod", L"Model files", tFileMode::read );
	if ( m_wstrPathOfOpenModel != L"" )
		Read( m_wstrPathOfOpenModel );
}

void NNetModelStorage::writeModel( )
{
	std::wofstream modelFile( m_wstrPathOfOpenModel );
	Write( modelFile );
	modelFile.close( );
}

void NNetModelStorage::SaveModelAs( )
{
	if ( m_wstrPathOfOpenModel == L"" )
		m_wstrPathOfOpenModel = GetPathOfExecutable( );

	m_wstrPathOfOpenModel = AskForFileName( m_wstrPathOfOpenModel, L"*.mod", L"Model files", tFileMode::write );

	writeModel( );
}

void NNetModelStorage::SaveModel( )
{
	if ( m_wstrPathOfOpenModel == L"" )
	{
		SaveModelAs( );
	}
	else
	{
		writeModel( );
	}
}
