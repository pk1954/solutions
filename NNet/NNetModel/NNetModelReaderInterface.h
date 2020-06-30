// NNetModelReaderInterface.h 
//
// NNetModel
//
// Interface for reader threads

#pragma once

#include "MoreTypes.h"
#include "ShapeType.h"
#include "ShapeId.h"
#include "Shape.h"
#include "tHighlightType.h"

class DrawContext;
class ObserverInterface;
class NNetModel;
class Param;
class Pipe;

class NNetModelReaderInterface
{
public:

	void Start( NNetModel * const );
	void Stop(); 

	NNetModel const & CopyModel() const;
	bool      const IsEqual( NNetModel const & ) const;

	bool      const AnyShapesSelected         ( )               const;	
	bool      const IsSelected                ( ShapeId const ) const;
	ShapeType const GetShapeType              ( ShapeId const ) const;
	fHertz    const GetPulseFreq              ( ShapeId const ) const;
	size_t    const GetNrOfSegments           ( ShapeId const ) const;
	bool      const HasTriggerSound           ( ShapeId const ) const;
	Hertz     const GetTriggerSoundFrequency  ( ShapeId const ) const;
	MilliSecs const GetTriggerSoundDuration   ( ShapeId const ) const;
	mV        const GetVoltage                ( ShapeId const ) const;
	mV        const GetVoltage                ( ShapeId const, MicroMeterPoint const & ) const;
	bool      const HasIncoming               ( ShapeId const ) const;
	bool      const HasOutgoing               ( ShapeId const ) const;
	size_t    const GetNrOfOutgoingConnections( ShapeId const ) const;
	size_t    const GetNrOfIncomingConnections( ShapeId const ) const;
	bool      const ConnectsTo( ShapeId const, ShapeId const ) const;

	MicroMeterPoint const GetShapePos( ShapeId const ) const;
	MicroMeterRect  const GetEnclosingRect( ) const;

	fMicroSecs GetSimulationTime( ) const;

	unsigned long const GetNrOfShapes      ( ) const;
	unsigned long const GetNrOfInputNeurons( ) const;
	unsigned long const GetNrOfNeurons     ( ) const;
	unsigned long const GetNrOfKnots       ( ) const;
	unsigned long const GetNrOfPipes       ( ) const;

	void DrawExterior( ShapeId const, DrawContext const &, tHighlightType const ) const;
	void DrawInterior( ShapeId const, DrawContext const & ) const;

	ShapeId const FindShapeAt( MicroMeterPoint const &, ShapeCrit const & ) const;

	template <typename T> bool IsOfType( ShapeId const id ) const 
	{ 
		return T::TypeFits( GetShapeType( id ) ); 
	}

private:
	NNetModel const * m_pModel;

	bool const isConnectedTo    ( ShapeId const, ShapeId const ) const;
	bool const isConnectedToPipe( ShapeId const, ShapeId const ) const;
};
