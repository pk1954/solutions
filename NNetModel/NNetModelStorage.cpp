// NNetModelStorage.cpp
//
// NNetModel

#include "stdafx.h"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>
#include "assert.h"
#include "NNetModel.h"
#include "NNetModelStorage.h"

using std::endl;
using std::put_time;

static unsigned int MAJOR_VERSION { 0 };
static unsigned int MINOR_VERSION { 1 };

void WriteModel( NNetModel const & model, wostream & out )
{
	struct tm tmDest;
	time_t t = std::time( nullptr );
	localtime_s( &tmDest, &t );

	out << L"# NNetModel" << endl;
	out << L"# created " << put_time( &tmDest, L"%d-%m-%Y %H-%M-%S" ) << endl;
	out << L"Version " << MAJOR_VERSION << L"." << MINOR_VERSION  << endl;
	model.Apply2AllShapes( [&]( Shape & shape ) { if ( shape.IsDefined() ) WriteShape( model, out, shape ); } );
	model.Apply2AllParameters
	( 
		[&]( tParameter const & param ) 
		{
			if ( param != tParameter::pulseRate )
				out << L"Parameter \"" << model.GetParameterName( param ) << L"\" " << model.GetParameter( param, nullptr ) << endl; 
		}
	);
	model.Apply2AllInputNeurons
	(
		[&]( InputNeuron & inpNeuron )
		{ 
			out << L"InputNeuron "
				<< inpNeuron.GetId().GetValue()
				<< L" Parameter \"" 
				<< model.GetParameterName( tParameter::pulseRate ) 
				<< L"\" " 
				<< model.GetParameter( tParameter::pulseRate, & inpNeuron ); 
		}
	);

	out << endl;
}

void WritePipeline( wostream & out, Shape const & shape )
{
	assert( shape.GetShapeType() == tShapeType::pipeline );
	Pipeline const & pipe { static_cast<Pipeline const &>( shape ) };
	out << L"pipeline ";
	out << pipe.GetStartKnot().GetValue() << L" " << pipe.GetEndKnot().GetValue();
}

void WriteMicroMeter( wostream & out, MicroMeter const & um )
{
	out << um.GetValue() << L" ";
}

void WriteMicroMeterPoint( wostream & out, MicroMeterPoint const & pnt )
{
	WriteMicroMeter( out, pnt.GetX() );
	WriteMicroMeter( out, pnt.GetY() );
}

void WriteShape( NNetModel const & model, wostream & out, Shape & shape )
{
	out << L"Shape " << shape.GetId().GetValue() << L" ";
	switch ( shape.GetShapeType( ) )
	{
		case tShapeType::inputNeuron:
			out << L"inputNeuron ";
			WriteMicroMeterPoint( out, static_cast<BaseKnot &>( shape ).GetPosition() );
			break;

		case tShapeType::outputNeuron:
			out << L"outputNeuron ";
			WriteMicroMeterPoint( out, static_cast<BaseKnot &>( shape ).GetPosition() );
			break;

		case tShapeType::neuron:
			out << L"neuron ";
			WriteMicroMeterPoint( out, static_cast<BaseKnot &>( shape ).GetPosition() );
			break;

		case tShapeType::pipeline:
			WritePipeline( out, shape );
			break;

		case tShapeType::knot:
			out << L"knot ";
			WriteMicroMeterPoint( out, static_cast<BaseKnot &>( shape ).GetPosition() );
			break;

		default:
			assert( false );
			break;
	}
	out << endl;
}

void ReadModel ( NNetModel & model, wistream & in )
{

}
