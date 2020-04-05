// NNetModelStorage.cpp
//
// NNetSimu

#include "stdafx.h"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <filesystem>
#include "assert.h"
#include "SCRIPT.H"
#include "symtab.h"
#include "errhndl.h"
#include "MoreTypes.h"
#include "NNetModel.h"
#include "Preferences.h"
#include "InputNeuron.h"
#include "win32_script.h"
#include "win32_NNetAppMenu.h"
#include "NNetModelStorage.h"

using std::wcout;
using std::endl;
using std::put_time;
using std::wofstream;
using std::filesystem::exists;

//static float const PROTOCOL_VERSION { 1.4f };
static float const PROTOCOL_VERSION { 1.5f };   // pipeline renamed to pipe

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
		ShapeId   const idFromScript{ script.ScrReadLong() };
		ShapeType const shapeType   { static_cast<ShapeType::Value>(script.ScrReadInt( )) };
		Shape         * pShape      { nullptr };

		script.ScrReadSpecial( L'(' );
		if ( shapeType.IsPipeType() )
		{
			ShapeId const idStart { script.ScrReadLong() };
			script.ScrReadSpecial( L'-' );
			script.ScrReadSpecial( L'>' );
			ShapeId const idEnd { script.ScrReadLong() };
			if ( idStart == idEnd )
			{
				wcout << "+++ Pipe has identical start and end point" << endl;
				wcout << "+++ " << idFromScript << L": " << idStart << L" -> " << idEnd << endl;
				wcout << "+++ Pipe ignored" << endl;
			}
			else
			{ 
				Pipe     * const pPipe  { new Pipe( ) };
				BaseKnot * const pStart { m_pModel->GetTypedShape<BaseKnot>( idStart ) };
				BaseKnot * const pEnd   { m_pModel->GetTypedShape<BaseKnot>( idEnd   ) };

				pPipe->SetId( idFromScript );

				pStart->AddOutgoing( pPipe );
				pPipe->SetStartKnot( pStart );

				pEnd->AddIncoming( pPipe );
				pPipe->SetEndKnot( pEnd );

				pShape = pPipe;
			}
		}
		else 
		{
			MicroMeter      const xCoord { CastToFloat( script.ScrReadFloat() ) };
			script.ScrReadSpecial( L'|' );
			MicroMeter      const yCoord { CastToFloat( script.ScrReadFloat() ) };
			MicroMeterPoint const umPosition( xCoord, yCoord );
			switch ( shapeType.GetValue() )
			{
			case ShapeType::Value::inputNeuron:
				pShape = new InputNeuron( umPosition );
				break;

			case ShapeType::Value::neuron:
				pShape = new Neuron( umPosition );
				break;

			case ShapeType::Value::knot:
				pShape = new Knot( umPosition );
				break;

			default:
				assert( false );
				break;
			}
		}
		script.ScrReadSpecial( L')' );

		if ( pShape )
		{
			m_pModel->SetShape( pShape, idFromScript );
			pShape->SetId( idFromScript );
		}
		#ifndef NDEBUG
			m_pModel->CheckConsistency();
		#endif
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

class WrapTriggerSound : public Script_Functor
{
public:
	WrapTriggerSound( NNetModel * const pNNetModel ) :
		m_pModel( pNNetModel )
	{ };

	virtual void operator() ( Script & script ) const 
	{
		ShapeId const id      { script.ScrReadLong () };
		Neuron      * pNeuron { m_pModel->GetTypedShape<Neuron>( id ) };
		Hertz   const freq    { script.ScrReadUlong() };
		script.ScrReadString( L"Hertz" );
		MilliSecs const msec { script.ScrReadUlong() };
		script.ScrReadString( L"msec" );
		pNeuron->SetTriggerSoundOn( true );
		pNeuron->SetTriggerSoundFrequency( freq );
		pNeuron->SetTriggerSoundDuration ( msec );
	}

private:
	NNetModel * m_pModel;
};

void NNetModelStorage::prepareForReading( NNetModel * const pModel )
{
#define DEF_NNET_FUNC(name) SymbolTable::ScrDefConst( L#name, new Wrap##name##( pModel ) )
	DEF_NNET_FUNC( Protocol );
	DEF_NNET_FUNC( GlobalParameter );
	DEF_NNET_FUNC( ShapeParameter );
	DEF_NNET_FUNC( NrOfShapes );
	DEF_NNET_FUNC( CreateShape );
	DEF_NNET_FUNC( TriggerSound );
#undef DEF_NET_FUNC

	ShapeType::Apply2All
	( 
		[&]( ShapeType const & type ) 
		{
			SymbolTable::ScrDefConst
			( 
				ShapeType::GetName( type.GetValue() ), 
				static_cast<unsigned long>(type.GetValue()) 
			);
		}
	);

	///// Legacy /////
	SymbolTable::ScrDefConst( L"pipeline", static_cast<unsigned long>(ShapeType::Value::pipe) );  // support older protocol version
    ///// end Legacy /////

	Apply2AllParameters
	( 
		[&]( tParameter const & param ) 
		{
			SymbolTable::ScrDefConst
			( 
				GetParameterName( param ), 
				static_cast<unsigned long>( param ) 
			);
		}
	);

#undef DEF_NNET_FUNC

	m_bPreparedForReading = true;
}

bool NNetModelStorage::Read( NNetModel & model, wstring const wstrPath )
{
	if ( ! m_bPreparedForReading )
		prepareForReading( & model );

	wstring const wstrModelFilePath { ( wstrPath == L"" ) ? m_wstrPathOfOpenModel : wstrPath };
	if ( ! exists( wstrModelFilePath ) )
	{
		MessageBox( nullptr, wstrModelFilePath.c_str(), L"Could not find model file", MB_OK );
		return false;
	}

	model.ResetModel();

	wcout << L"** NNet model file " << wstrModelFilePath << endl;

	if ( ! Script::ProcessScript( wstrModelFilePath ) )
	{
		MessageBox( nullptr, wstrModelFilePath.c_str(), L"Error in model file. Using default model.", MB_OK );
		return false;
	}

	model.ModelSaved( );
	m_wstrPathOfOpenModel = wstrModelFilePath;
	NNetAppMenu::SetAppTitle( m_wstrPathOfOpenModel );
	return true;
}

////////////////////////// Write /////////////////////////////////////////////

void NNetModelStorage::Write( NNetModel const & model, wostream & out )
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

	Apply2GlobalParameters
	( 
		[&]( tParameter const & param ) 
		{
			out << L"GlobalParameter " << GetParameterName( param ) << L" = "
			<< model.GetParameterValue( param ) 
			<< endl; 
		}
	);

	m_CompactIds.resize( model.GetSizeOfShapeList() );
	ShapeId idCompact( 0 );
	for ( int i = 0; i < m_CompactIds.size( ); ++i )
	{
		m_CompactIds[ i ] = model.GetConstShape( ShapeId( i ) )
			              ? idCompact++
			              : NO_SHAPE;
	}

	out << endl;
	out << L"NrOfShapes = " << m_CompactIds.size() << endl;
	out << endl;

	model.Apply2All<BaseKnot>( [&]( BaseKnot & shape ) { WriteShape( out, shape ); } );
	model.Apply2All<Pipe>( [&]( Pipe & shape ) { WriteShape( out, shape ); } );

	out << endl;

	model.Apply2All<InputNeuron>
	(
		[&]( InputNeuron & inpNeuron )
		{ 
			out << L"ShapeParameter InputNeuron " << getCompactIdVal( inpNeuron.GetId() ) << L" "
				<< GetParameterName( tParameter::pulseRate ) 
				<< L" = " << model.GetPulseRate( & inpNeuron )
     			<< endl; 
		}
	);

	out << endl;

	model.Apply2All<Neuron>
	( 
		[&]( Neuron & neuron ) 
		{ 
			if ( neuron.HasTriggerSound( ) )
			{
				out << L"TriggerSound " << getCompactIdVal( neuron.GetId() ) << L" "
					<< neuron.GetTriggerSoundFrequency() << L" Hertz "
					<< neuron.GetTriggerSoundDuration()  << L" msec "
					<< endl; 
			}
		} 
	);

	model.ModelSaved( );
}

void NNetModelStorage::WritePipe( wostream & out, Shape const & shape )
{
	Pipe const & pipe { static_cast<Pipe const &>( shape ) };
	out << getCompactIdVal( pipe.GetStartKnotId() ) << L"->" << getCompactIdVal( pipe.GetEndKnotId() ) ;
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
		switch ( shape.GetShapeType( ).GetValue() )
		{
			case ShapeType::Value::inputNeuron:
			case ShapeType::Value::neuron:
			case ShapeType::Value::knot:
				WriteMicroMeterPoint( out, static_cast<BaseKnot &>( shape ).GetPosition() );
				break;

			case ShapeType::Value::pipe:
				WritePipe( out, shape );
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

int NNetModelStorage::AskSave( )
{
	return MessageBox( nullptr, L"Save now?", L"Unsaved changes", MB_YESNOCANCEL );
}

bool NNetModelStorage::AskAndSave( NNetModel const & model )
{
	if ( model.HasModelChanged() )
	{
		int iRes = AskSave( );
		if ( iRes == IDYES )
			SaveModel( model );
		else if ( iRes == IDNO )
			return true;
		else if ( iRes == IDCANCEL )
			return false;
	}
	return true;
}

bool NNetModelStorage::AskModelFile( )
{
	wstring wstrPath = AskForFileName( GetPathOfExecutable( ), L"*.mod", L"Model files", tFileMode::read );
	if ( wstrPath != L"" )
	{
		m_wstrPathOfOpenModel = wstrPath;
		return true;
	}
	return false;
}

void NNetModelStorage::writeModel( NNetModel const & model )
{
	wofstream modelFile( m_wstrPathOfOpenModel );
	Write( model, modelFile );
	modelFile.close( );
	NNetAppMenu::SetAppTitle( m_wstrPathOfOpenModel );
}

bool NNetModelStorage::SaveModelAs( NNetModel const & model )
{
	if ( m_wstrPathOfOpenModel == L"" )
		m_wstrPathOfOpenModel = GetPathOfExecutable( );

	m_wstrPathOfOpenModel = AskForFileName( m_wstrPathOfOpenModel, L"*.mod", L"Model files", tFileMode::write );

	bool const bRes = m_wstrPathOfOpenModel != L"";
	if ( bRes )
		writeModel( model );
	return bRes;
}

bool NNetModelStorage::SaveModel( NNetModel const & model )
{
	if ( m_wstrPathOfOpenModel == L"" )
	{
		return SaveModelAs( model );
	}
	else
	{
		writeModel( model );
		return true;
	}
}

void NNetModelStorage::ResetModelPath( ) 
{ 
	m_wstrPathOfOpenModel = L""; 
	NNetAppMenu::SetAppTitle( m_wstrPathOfOpenModel );
}
