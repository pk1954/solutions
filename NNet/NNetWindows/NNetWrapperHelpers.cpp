// NNetWrapperHelpers.cpp : wrapper functions for unit tests and scripting of application
//
// NNetWindows

#include "stdafx.h"
#include "assert.h"
#include "ERRHNDL.H"
#include "SCRIPT.H"
#include "NNetModel.H"
#include "NNetWrapperHelpers.h"

ShapeId ScrReadShapeId( Script & script )
{
	ShapeId const id { static_cast<ShapeId>( script.ScrReadLong( ) ) };
	return id;
}

MicroMeter ScrReadMicroMeter( Script & script )
{
	float const fValue = CastToFloat( script.ScrReadFloat( ) );
	if ( fabs(fValue) > MAX_MICRO_METER.GetValue() )
		ScriptErrorHandler::throwError( 777, L"MicroMeter value too big" );
	return MicroMeter( fValue );
}

MicroMeterPoint ScrReadMicroMeterPoint( Script & script )
{
	script.ScrReadSpecial( '(' );
	MicroMeter const x(ScrReadMicroMeter( script ));
	script.ScrReadSpecial( '/' );
	MicroMeter const y(ScrReadMicroMeter( script ));
	script.ScrReadSpecial( ')' );
	return MicroMeterPoint( x, y );
}
