// Pipeline.h
//
// NNetModel

#pragma once

#include <vector>
#include "MoreTypes.h"
#include "NNetParameters.h"
#include "Shape.h"
#include "Segment.h"

using std::vector;

class GraphicsInterface;
class PixelCoordsFp;
class BaseKnot;

class Pipeline : public Shape
{
public:
	Pipeline( meterPerSec const = STD_IMPULSE_SPEED );

	void SetStartKnot( ShapeId const );
	void SetEndKnot  ( ShapeId const );

	void Resize( );

	void SetPulseSpeed( meterPerSec const newSpeed )
	{
		m_impulseSpeed = newSpeed;
		m_initialized = false;
		initialize( );
	}

	meterPerSec GetPulseSpeed  ( ) const { return m_impulseSpeed;	}
	ShapeId     GetStartKnot   ( ) const { return m_idKnotStart;  }
	ShapeId     GetEndKnot     ( ) const { return m_idKnotEnd;    }
	size_t      GetNrOfSegments( ) const { return m_potential.size(); }

	MicroMeterPoint GetStartPoint( ) const; 
	MicroMeterPoint GetEndPoint  ( ) const; 
	MicroMeter      GetWidth     ( ) const; 

	void CheckConsistency( ) const;

	virtual void Step( );
	virtual void Prepare( );
	virtual void DrawExterior( ) const;
	virtual void DrawInterior( ) const;
	virtual void MoveTo( MicroMeterPoint const & ) {}
	virtual bool IsPointInShape(  MicroMeterPoint const & ) const;
	virtual mV   GetNextOutput( ) const;

private:
	void initialize( );

	bool        m_initialized;
	ShapeId     m_idKnotStart;
	ShapeId     m_idKnotEnd;
	MicroMeter  m_width;
	vector<mV>  m_potential;
	meterPerSec m_impulseSpeed;
};

Pipeline const * Cast2Pipeline( Shape const * );
Pipeline       * Cast2Pipeline( Shape       * );
