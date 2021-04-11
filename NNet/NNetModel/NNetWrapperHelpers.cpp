// NNetWrapperHelpers.cpp : wrapper functions for unit tests and scripting of application
//
// NNetWindows

#include "stdafx.h"
#include "ERRHNDL.H"
#include "SCRIPT.H"
#include "ShapeId.h"
#include "NNetWrapperHelpers.h"

MicroMeter ScrReadMicroMeter(Script & script)
{
	float const fValue = Cast2Float( script.ScrReadFloat() );
	if ( fabs(fValue) > MAX_MICRO_METER.GetValue() )
		throw ScriptErrorHandler::ScriptException( 777, L"MicroMeter value too big" );
	return MicroMeter( fValue );
}

MicroMeterPoint ScrReadMicroMeterPoint(Script & script)
{
	script.ScrReadSpecial( MicroMeterPoint::OPEN_BRACKET );
	MicroMeter const x(ScrReadMicroMeter( script ));
	script.ScrReadSpecial( MicroMeterPoint::SEPARATOR );
	MicroMeter const y(ScrReadMicroMeter( script ));
	script.ScrReadSpecial( MicroMeterPoint::CLOSE_BRACKET );
	return MicroMeterPoint( x, y );
}

MicroMeterCircle ScrReadMicroMeterCircle(Script & script)
{
	script.ScrReadSpecial( MicroMeterCircle::OPEN_BRACKET );
	MicroMeterPoint umCenter { ScrReadMicroMeterPoint( script ) };
	script.ScrReadSpecial( MicroMeterCircle::SEPARATOR );
	MicroMeter      umRadius { ScrReadMicroMeter( script ) };
	script.ScrReadSpecial( MicroMeterCircle::CLOSE_BRACKET );
	return MicroMeterCircle( umCenter, umRadius );
}

MicroMeterPosDir ScrReadMicroMeterPosDir(Script & script)
{
	script.ScrReadSpecial( MicroMeterPosDir::OPEN_BRACKET );
	MicroMeterPoint const umPnt(ScrReadMicroMeterPoint( script ));
	script.ScrReadSpecial( MicroMeterPosDir::SEPARATOR );
	Radian const rad(Cast2Float(script.ScrReadFloat()));
	script.ScrReadSpecial( MicroMeterPosDir::CLOSE_BRACKET );
	return MicroMeterPosDir( umPnt, rad );
}

MicroMeterPointVector ScrReadMicroMeterPointVector(Script& script)
{
	MicroMeterPointVector umPntVector;
	script.ScrReadSpecial( MicroMeterPointVector::OPEN_BRACKET );
	int const iNrOfElements { script.ScrReadInt() };
	script.ScrReadSpecial( L':' );
	for (int i = 0;;)
	{
		umPntVector.Add( ScrReadMicroMeterPosDir(script) );
		if (++i == iNrOfElements )
			break;
		script.ScrReadSpecial( MicroMeterPointVector::SEPARATOR );
	}
	script.ScrReadSpecial( MicroMeterPointVector::CLOSE_BRACKET );
	return umPntVector;
}

unique_ptr<ShapeIdList> ScrReadShapeIdList(Script& script)
{
	unique_ptr<ShapeIdList> upShapeIds  { make_unique<ShapeIdList>() };
	script.ScrReadSpecial( ShapeIdList::OPEN_BRACKET );
	int const iNrOfElements { script.ScrReadInt() };
	script.ScrReadSpecial( L':' );
	for (int i = 0;;)
	{
		upShapeIds->Add( ScrReadShapeId(script) );
		if (++i == iNrOfElements )
			break;
		script.ScrReadSpecial( ShapeIdList::SEPARATOR );
	}
	script.ScrReadSpecial( ShapeIdList::CLOSE_BRACKET );
	return move(upShapeIds);
}