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
#include "NNetModel.h"
#include "NNetModelStorage.h"

using std::wcout;
using std::endl;
using std::put_time;

static float const PROTOCOL_VERSION { 1.3f };

////////////////////////// Read /////////////////////////////////////////////

class WrapProtocol : public Script_Functor
{
public:
	WrapProtocol( NNetModelStorage * const pNNetModelStorage ) :
		m_pNNetModelStorage( pNNetModelStorage )
	{ };

	virtual void operator() ( Script & script ) const 
	{
		script.ScrReadString( L"version" );
		double dVersion = script.ScrReadFloat();
	}

private:
	NNetModelStorage * m_pNNetModelStorage;
};

class WrapCreateShape : public Script_Functor
{
public:
	WrapCreateShape( NNetModelStorage * const pNNetModelStorage ) :
		m_pNNetModelStorage( pNNetModelStorage )
	{ };

	virtual void operator() ( Script & script ) const
	{
		ShapeId           idModel;
		NNetModel * const pModel    { m_pNNetModelStorage->GetModel() };
		tShapeType  const shapeType { static_cast<tShapeType>(script.ScrReadInt( )) };
		script.ScrReadSpecial( L'(' );
		switch ( shapeType )
		{
		case tShapeType::inputNeuron:
			idModel = pModel->NewShape<InputNeuron>( readPoint( script ) );
			break;

		case tShapeType::outputNeuron:
			idModel = pModel->NewShape<OutputNeuron>( readPoint( script ) );
			break;

		case tShapeType::neuron:
			idModel = pModel->NewShape<Neuron>( readPoint( script ) );
			break;

		case tShapeType::knot:
			idModel = pModel->NewShape<Knot>( readPoint( script ) );
			break;

		case tShapeType::pipeline:
		{
			ShapeId const idStart { script.ScrReadLong() };
			script.ScrReadSpecial( L'-' );
			ShapeId const idEnd { script.ScrReadLong() };
			idModel = pModel->NewPipeline( idStart, idEnd );
		}
		break;

		default:
			assert( false );
			break;
		}
		script.ScrReadSpecial( L')' );
		script.ScrReadString( L"id" );
		script.ScrReadSpecial( L'=' );
		ShapeId idFromScript { script.ScrReadLong() };
		assert( idFromScript == idModel );
	}

private:
	MicroMeterPoint const readPoint( Script & script ) const
	{
		MicroMeter const xCoord { CastToFloat( script.ScrReadFloat() ) };
		script.ScrReadSpecial( L'|' );
		MicroMeter const yCoord { CastToFloat( script.ScrReadFloat() ) };
		return MicroMeterPoint( xCoord, yCoord );
	}

	NNetModelStorage * m_pNNetModelStorage;
};

class WrapGlobalParameter : public Script_Functor
{
public:
	WrapGlobalParameter( NNetModelStorage * const pNNetModelStorage ) :
		m_pNNetModelStorage( pNNetModelStorage )
	{ };

	virtual void operator() ( Script & script ) const 
	{
		NNetModel * const pModel { m_pNNetModelStorage->GetModel() };
		tParameter  const param( static_cast< tParameter >( script.ScrReadUint() ) );
		script.ScrReadSpecial( L'=' );
		float const fValue { CastToFloat( script.ScrReadFloat() ) };
		pModel->SetParameter( pModel->GetHighlightedShapeId( ), param, fValue );
	}

private:
	NNetModelStorage * m_pNNetModelStorage;
};

class WrapShapeParameter : public Script_Functor
{
public:
	WrapShapeParameter( NNetModelStorage * const pNNetModelStorage ) :
		m_pNNetModelStorage( pNNetModelStorage )
	{ };

	virtual void operator() ( Script & script ) const 
	{
		script.ScrReadString( L"InputNeuron" );
		ShapeId const id ( script.ScrReadUint() );
		tParameter const param( static_cast< tParameter >( script.ScrReadUint() ) );
		script.ScrReadSpecial( L'=' );
		float const fValue { CastToFloat( script.ScrReadFloat() ) };
		m_pNNetModelStorage->GetModel()->SetParameter( id, param, fValue );
	}

private:
	NNetModelStorage * m_pNNetModelStorage;
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
	: m_pModel( pModel )
{
#define DEF_NNET_FUNC(name) SymbolTable::ScrDefConst( L#name, new Wrap##name##( this ) )

	DEF_NNET_FUNC( Protocol );
	DEF_NNET_FUNC( CreateShape );
	DEF_NNET_FUNC( GlobalParameter );
	DEF_NNET_FUNC( ShapeParameter );

	DEF_ULONG_CONST( tShapeType::inputNeuron  );
	DEF_ULONG_CONST( tShapeType::outputNeuron );
	DEF_ULONG_CONST( tShapeType::neuron       );
	DEF_ULONG_CONST( tShapeType::pipeline     );
	DEF_ULONG_CONST( tShapeType::knot         );

	DEF_ULONG_CONST( tParameter::pulseRate        );
	DEF_ULONG_CONST( tParameter::pulseSpeed       );
	DEF_ULONG_CONST( tParameter::pulseWidth       );
	DEF_ULONG_CONST( tParameter::signalLoss       );
	DEF_ULONG_CONST( tParameter::threshold        );
	DEF_ULONG_CONST( tParameter::peakVoltage      );
	DEF_ULONG_CONST( tParameter::refractoryPeriod );

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

	out << L"Protocol version " << PROTOCOL_VERSION << endl;

	m_pModel->Apply2All<Shape>
	( 
		[&]( Shape & shape ) 
		{ 
			WriteShape( out, shape ); 
		} 
	);

	m_pModel->Apply2AllParameters
	( 
		[&]( tParameter const & param ) 
		{
			if ( param != tParameter::pulseRate )
				out << L"GlobalParameter " << m_pModel->GetParameterLiteral( param ) << L" = "
				<< m_pModel->GetParameterValue( param, nullptr ) 
			    << endl; 
		}
	);

	m_pModel->Apply2All<InputNeuron>
	(
		[&]( InputNeuron & inpNeuron )
		{ 
			out << L"ShapeParameter InputNeuron " << inpNeuron.GetId().GetValue() << L" "
				<< m_pModel->GetParameterLiteral( tParameter::pulseRate ) 
				<< L" = " << m_pModel->GetParameterValue( tParameter::pulseRate, & inpNeuron )
     			<< endl; 
		}
	);
}

void NNetModelStorage::WritePipeline( wostream & out, Shape const & shape )
{
	Pipeline const & pipe { static_cast<Pipeline const &>( shape ) };
	out << L"(" << pipe.GetStartKnot().GetValue() << L"-" << pipe.GetEndKnot().GetValue() << L")";
}

void NNetModelStorage::WriteMicroMeterPoint( wostream & out, MicroMeterPoint const & pnt )
{
	out << L"(" << pnt.GetXvalue() << L"|" << pnt.GetYvalue() << L")";
}

void NNetModelStorage::WriteShape( wostream & out, Shape & shape )
{
	if ( shape.IsDefined() )
	{
		out << L"CreateShape tShapeType::" << shape.GetName() << L" ";;
		switch ( shape.GetShapeType( ) )
		{
			case tShapeType::inputNeuron:
			case tShapeType::outputNeuron:
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
		out << L" id = " << shape.GetId().GetValue();
		out << endl;
	}
}
