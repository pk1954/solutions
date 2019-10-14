// Pipeline.h
//
// NNetModel

#pragma once

#include <vector>
#include "MoreTypes.h"
#include "Shape.h"
#include "Segment.h"

using std::vector;

class GraphicsInterface;
class PixelCoordsFp;
class BaseKnot;

class Pipeline : public Shape
{
public:
	Pipeline( meterPerSec const );

	void SetStartKnot( NNetModel &, ShapeId const );
	void SetEndKnot  ( NNetModel &, ShapeId const );

	void Resize( NNetModel & );

	void SetPulseSpeed( NNetModel & model, meterPerSec const newSpeed )
	{
		m_impulseSpeed = newSpeed;
		m_initialized = false;
		initialize( model );
	}

	meterPerSec GetPulseSpeed( ) const
	{
		return m_impulseSpeed;
	}

	MicroMeterPoint GetStartPoint( NNetModel const & ) const; 
	MicroMeterPoint GetEndPoint  ( NNetModel const & ) const; 
	MicroMeter      GetWidth     ( ) const; 

	virtual void Step( );
	virtual void Prepare( NNetModel & );

	virtual void DrawExterior( NNetModel const &, GraphicsInterface &, PixelCoordsFp const & ) const;
	virtual void DrawInterior( NNetModel const &, GraphicsInterface &, PixelCoordsFp const & ) const;
	virtual void MoveTo( NNetModel &, MicroMeterPoint const & );

	virtual bool IsPointInShape( NNetModel const &, MicroMeterPoint const & ) const;
	virtual mV   GetNextOutput( ) const;

private:
	MicroMeter distance( MicroMeterPoint const &, MicroMeterPoint const & );
	void       initialize( NNetModel & );
	COLORREF   pulseColor( mV const ) const;

	bool m_initialized;
	
	ShapeId    m_idKnotStart;
	ShapeId    m_idKnotEnd;
	MicroMeter m_width;

	vector<mV>  m_potential;
	meterPerSec m_impulseSpeed;
};

Pipeline const * Cast2Pipeline( Shape const * );
Pipeline       * Cast2Pipeline( Shape       * );
